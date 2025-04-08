import serial

PORT = "COM9"
BAUD = 921600

def count_bits(n):
    return bin(n).count('1')

def decode_adc_frame(data):
    channel_map = data[1]
    num_channels = count_bits(channel_map)
    expected_size = 1 + 1 + (num_channels * 2) + 2

    if len(data) < expected_size:
        return None

    adc_vals = []
    index = 2

    for i in range(8):
        if (channel_map >> i) & 0x01:
            raw = int.from_bytes(data[index:index+2], 'big')
            adc_vals.append((i, raw))
            index += 2

    timestamp = int.from_bytes(data[index:index+2], 'big')
    return adc_vals, timestamp, expected_size

def decode_digital_frame(data):
    pin_map = data[1]
    num_channels = count_bits(pin_map)
    expected_size = 1 + 1 + num_channels + 2

    if len(data) < expected_size:
        return None

    digital_vals = []
    index = 2

    for i in range(8):
        if (pin_map >> i) & 0x01:
            state = data[index]
            digital_vals.append((i, state))
            index += 1

    timestamp = int.from_bytes(data[index:index+2], 'big')
    return digital_vals, timestamp, expected_size

def main():
    buffer = bytearray()
    ser = serial.Serial(PORT, BAUD, timeout=0.05)
    print(f"Listening on {PORT} at {BAUD} baud...\n")

    try:
        while True:
            chunk = ser.read(64)
            buffer.extend(chunk)

            while len(buffer) >= 4:
                header = buffer[0]

                if header == 0xAA:
                    # ADC Frame
                    channel_map = buffer[1]
                    num_channels = count_bits(channel_map)
                    frame_size = 1 + 1 + (num_channels * 2) + 2

                    if len(buffer) < frame_size:
                        break

                    frame = buffer[:frame_size]
                    buffer = buffer[frame_size:]

                    result = decode_adc_frame(frame)
                    if result:
                        adc_vals, ts, _ = result
                        print(f"[{ts:05d}] ADC Values:", end=" ")
                        for ch, val in adc_vals:
                            print(f"A{ch}:{val:>4}", end="  ")
                        print()

                elif header == 0xAB:
                    # Digital Frame
                    pin_map = buffer[1]
                    num_channels = count_bits(pin_map)
                    frame_size = 1 + 1 + num_channels + 2

                    if len(buffer) < frame_size:
                        break

                    frame = buffer[:frame_size]
                    buffer = buffer[frame_size:]

                    result = decode_digital_frame(frame)
                    if result:
                        digital_vals, ts, _ = result
                        states = ["1" if any(p == i and s for p, s in digital_vals) else "0" for i in range(8)]
                        print(f"[{ts:05d}] Digital Pins: {' '.join(states)}")

                else:
                    buffer.pop(0)  # Skip unknown byte

    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        ser.close()

if __name__ == "__main__":
    main()
