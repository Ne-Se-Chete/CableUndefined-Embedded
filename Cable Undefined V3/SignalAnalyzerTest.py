import serial

PORT = "COM9"
BAUD = 921600

def count_bits(n):
    return bin(n).count('1')

def decode_frame(data):
    if data[0] != 0xAA:
        return None

    channel_map = data[1]
    num_channels = count_bits(channel_map)
    expected_size = 1 + 1 + (num_channels * 2) + 2  # header + map + ADC + timestamp

    if len(data) < expected_size:
        return None

    adc_vals = []
    index = 2  # start after 0xAA and channel_map

    for i in range(8):
        if (channel_map >> i) & 0x01:
            raw = int.from_bytes(data[index:index+2], 'big')
            adc_vals.append((i, raw))  # store channel index + value
            index += 2

    timestamp = int.from_bytes(data[index:index+2], 'big')
    return adc_vals, timestamp, expected_size

def main():
    with serial.Serial(PORT, BAUD, timeout=0.1) as ser:
        buffer = bytearray()

        while True:
            chunk = ser.read(64)
            buffer.extend(chunk)

            while len(buffer) >= 4:  # minimum frame size
                if buffer[0] != 0xAA:
                    buffer.pop(0)
                    continue

                channel_map = buffer[1]
                num_channels = count_bits(channel_map)
                frame_size = 1 + 1 + num_channels * 2 + 2

                if len(buffer) < frame_size:
                    break  # wait for full frame

                frame = buffer[:frame_size]
                buffer = buffer[frame_size:]

                result = decode_frame(frame)
                if result:
                    adc_vals, ts, _ = result
                    print(f"T:{ts} | " + " ".join(f"ADC{ch}:{val}" for ch, val in adc_vals))

if __name__ == "__main__":
    main()
