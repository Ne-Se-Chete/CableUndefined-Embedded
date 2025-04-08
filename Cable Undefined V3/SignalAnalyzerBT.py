import bluetooth
import threading
import sys

def count_bits(n):
    return bin(n).count('1')

def decode_frame(data):
    if data[0] != 0xAA:
        return None

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

def connect_bluetooth(name_filter="Cable Undefined V3"):
    print("🔍 Scanning for Bluetooth devices...")
    devices = bluetooth.discover_devices(duration=8, lookup_names=True)
    for addr, name in devices:
        if name and name_filter in name:
            print(f"✅ Found {name} at {addr}")
            sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
            sock.connect((addr, 1))  # Port 1 is default for BluetoothSerial
            print(f"🔌 Connected to {name} ({addr}) on port 1\n")
            return sock
    raise Exception("ESP32 device not found.")

def receive_loop(sock):
    buffer = bytearray()
    try:
        while True:
            chunk = sock.recv(64)
            buffer.extend(chunk)

            while len(buffer) >= 4:
                if buffer[0] != 0xAA:
                    buffer.pop(0)
                    continue

                channel_map = buffer[1]
                num_channels = count_bits(channel_map)
                frame_size = 1 + 1 + num_channels * 2 + 2

                if len(buffer) < frame_size:
                    break

                frame = buffer[:frame_size]
                buffer = buffer[frame_size:]

                result = decode_frame(frame)
                if result:
                    adc_vals, ts, _ = result
                    print(f"T:{ts} | " + " ".join(f"ADC{ch}:{val}" for ch, val in adc_vals))
    except Exception as e:
        print(f"\n❌ Error in receive loop: {e}")
        sock.close()

def send_loop(sock):
    try:
        while True:
            cmd = input("> ")
            if cmd.lower() in ("exit", "quit"):
                print("👋 Exiting.")
                sock.close()
                sys.exit()
            sock.send(cmd.encode("utf-8") + b'\n')
    except Exception as e:
        print(f"\n❌ Error in send loop: {e}")
        sock.close()

def main():
    try:
        sock = connect_bluetooth()

        threading.Thread(target=receive_loop, args=(sock,), daemon=True).start()
        send_loop(sock)

    except Exception as e:
        print(f"❌ Connection failed: {e}")

if __name__ == "__main__":
    main()
