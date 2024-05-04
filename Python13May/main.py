import tkinter as tk

# Define colors
colors = [
    "#FC0303",  # Light Red
    "#03FC39",  # Light Green
    "#FCF803",  # Light Blue
    "#FCF803",  # Light Yellow
    "#007099",  # Light Cyan
    "#FF36A1",  # Light Magenta
    "#858585",  # Light Gray
    "#FF8000"   # Orange
]


class Breadboard(tk.Tk):
    def __init__(self, main_rows, main_columns, small_rows, small_columns):
        super().__init__()
        self.main_rows = main_rows
        self.main_columns = main_columns
        self.small_rows = small_rows
        self.small_columns = small_columns
        self.title("Dual Breadboard")
        self.main_pins = [[False for _ in range(main_columns)] for _ in range(main_rows)]
        self.small_pins = [[False for _ in range(small_columns)] for _ in range(small_rows)]
        self.last_clicked = None  # Store the last pin clicked
        self.connections = []  # Store the connections
        self.create_widgets()

    def create_widgets(self):
        self.main_frame = tk.Frame(self, bd=2, relief=tk.RAISED)
        self.main_frame.pack(side=tk.BOTTOM, padx=20, pady=20)
        self.main_buttons = [[None for _ in range(self.main_columns)] for _ in range(self.main_rows)]
        
        count = 1
        for i in range(self.main_rows):
            for j in range(self.main_columns):
                label_text = str(count)
                self.main_buttons[i][j] = tk.Button(self.main_frame, text=label_text, width=8, height=4, relief="raised",
                                                    command=lambda row=i, column=j: self.toggle_pin(row, column, 'main'))
                self.main_buttons[i][j].grid(row=i, column=j, padx=4, pady=4)
                count += 1

        self.small_frame = tk.Frame(self, bd=2, relief=tk.RAISED)
        self.small_frame.pack(side=tk.TOP, padx=20, pady=20)
        self.small_buttons = [[None for _ in range(self.small_columns)] for _ in range(self.small_rows)]
        small_pin_numbers = [
            [8, 7, 6, 5],
            [1, 2, 3, 4]
        ]
        for i in range(self.small_rows):
            for j in range(self.small_columns):
                label_text = str(small_pin_numbers[i][j])
                self.small_buttons[i][j] = tk.Button(self.small_frame, text=label_text, width=8, height=4, relief="raised",
                                                     command=lambda row=i, column=j: self.toggle_pin(row, column, 'small'))
                self.small_buttons[i][j].grid(row=i, column=j, padx=4, pady=4)

    def toggle_pin(self, row, column, board_type):
        # Determine the correct button and pin state based on board type
        button = self.main_buttons[row][column] if board_type == 'main' else self.small_buttons[row][column]
        pin_state = self.main_pins[row][column] if board_type == 'main' else self.small_pins[row][column]

        # Toggle the visual state of the button
        if button.cget('relief') == 'sunken':
            button.config(relief="raised", bg="white")  # Reset color to default
            pin_state = False
        else:
            button.config(relief="sunken", bg=colors[7])  # Set color to orange for active connection
            pin_state = True
        
        # Update the internal state
        if board_type == 'main':
            self.main_pins[row][column] = pin_state
        else:
            self.small_pins[row][column] = pin_state

        # Update last clicked and connections
        current_pin = (board_type, row, column)
        self.remove_connection(current_pin)

        if pin_state:  # If we just set the pin, check for connections
            if self.last_clicked:
                if self.last_clicked != current_pin:
                    self.record_connection(self.last_clicked, current_pin)
                self.last_clicked = None
            else:
                self.last_clicked = current_pin

    def record_connection(self, pin1, pin2):
        connection = [pin1, pin2]
        self.connections.append(connection)
        print(f"Connection recorded: {connection}")

    def remove_connection(self, current_pin):
        for connection in self.connections[:]:
            if current_pin in connection:
                self.connections.remove(connection)
                print(f"Connection removed: {connection}")
                for pin in connection:
                    pin_type, pin_row, pin_col = pin
                    button = self.main_buttons[pin_row][pin_col] if pin_type == 'main' else self.small_buttons[pin_row][pin_col]
                    button.config(relief="raised", bg="#d9d9d9")  # Reset color to default
                    if pin_type == 'main':
                        self.main_pins[pin_row][pin_col] = False
                    else:
                        self.small_pins[pin_row][pin_col] = False

if __name__ == "__main__":
    root = Breadboard(main_rows=2, main_columns=12, small_rows=2, small_columns=4)
    root.mainloop()
