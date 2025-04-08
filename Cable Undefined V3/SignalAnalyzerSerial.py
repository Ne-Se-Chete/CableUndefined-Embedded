import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

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
    index = 2

    for i in range(8):
        if (channel_map >> i) & 0x01:
            raw = int.from_bytes(data[index:index+2], 'big')
            adc_vals.append((i, raw))
            index += 2

    timestamp = int.from_bytes(data[index:index+2], 'big')
    return adc_vals, timestamp, expected_size

# Set up plotting
history_len = 200
data_history = {ch: deque([0]*history_len, maxlen=history_len) for ch in range(8)}
x_data = deque(range(history_len), maxlen=history_len)

fig, ax = plt.subplots()
lines = {ch: ax.plot([], [])[0] for ch in range(8)}

ax.set_ylim(0, 4096)  # assuming 12-bit ADC
ax.set_xlim(0, history_len)
ax.set_xlabel("Samples")
ax.set_ylabel("ADC Value")

def init():
    for line in lines.values():
        line.set_data([], [])
    return lines.values()

def update_plot(frame):
    global ser, buffer

    chunk = ser.read(64)
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
            for ch, val in adc_vals:
                data_history[ch].append(val)
            for ch in range(8):
                if ch not in [c for c, _ in adc_vals]:
                    data_history[ch].append(data_history[ch][-1])  # repeat last value

            # Update lines
            for ch, line in lines.items():
                line.set_data(x_data, data_history[ch])

    return lines.values()

# Open serial port
ser = serial.Serial(PORT, BAUD, timeout=0.05)
buffer = bytearray()

ani = animation.FuncAnimation(fig, update_plot, init_func=init, interval=0, blit=True)
plt.tight_layout()
plt.show()

ser.close()
