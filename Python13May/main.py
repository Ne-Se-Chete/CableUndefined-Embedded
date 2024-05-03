import tkinter as tk



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
        self.create_widgets()

    def create_widgets(self):
        self.main_frame = tk.Frame(self, bd=2, relief=tk.RAISED)
        self.main_frame.pack(side=tk.BOTTOM, padx=20, pady=20)
        self.main_buttons = [[None for _ in range(self.main_columns)] for _ in range(self.main_rows)]
        
        # Create numbering for main breadboard
        count = 1
        for i in range(self.main_rows):
            for j in range(self.main_columns):
                label_text = str(count)
                self.main_buttons[i][j] = tk.Button(self.main_frame, text=label_text, width=8, height=4, relief="raised", command=lambda row=i, column=j: self.toggle_pin(row, column, 'main'))
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
                self.small_buttons[i][j] = tk.Button(self.small_frame, text=label_text, width=8, height=4, relief="raised", command=lambda row=i, column=j: self.toggle_pin(row, column, 'small'))
                self.small_buttons[i][j].grid(row=i, column=j, padx=4, pady=4)

    def toggle_pin(self, row, column, board_type):
        if board_type == 'main':
            pin_state = self.main_pins[row][column]
            self.main_pins[row][column] = not pin_state
            button = self.main_buttons[row][column]
            pin_number = row * self.main_columns + column + 1
        else:
            pin_state = self.small_pins[row][column]
            self.small_pins[row][column] = not pin_state
            button = self.small_buttons[row][column]
            # Adjust pin numbering for small board type
            if row == 0:
                pin_number = 8 - column
            else:
                pin_number = column + 1
        if pin_state:
            button.config(relief="raised")
            print(f"{board_type.capitalize()} Pin {pin_number} deactivated.")
        else:
            button.config(relief="sunken")
            print(f"{board_type.capitalize()} Pin {pin_number} activated.")
            



if __name__ == "__main__":
    root = Breadboard(main_rows=2, main_columns=12, small_rows=2, small_columns=4)
    root.mainloop()
