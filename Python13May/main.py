import tkinter as tk
from calculateConnections import *
import serial
import time
import openai


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

usedMUX1Pins = []
usedMUX2Pins = []

class Breadboard(tk.Tk):
    def __init__(self, main_rows, main_columns, small_rows, small_columns, serial_port, baud_rate=9600):
        super().__init__()
        #* Add sidebar that is on the right side of the window with width of 200px and height of the window. starts from the top and goes to the bottom
        self.create_sidebar()

        self.main_rows = main_rows
        self.main_columns = main_columns
        self.small_rows = small_rows
        self.small_columns = small_columns
        self.title("Cable Undefined Mini")
        self.main_pins = [[False for _ in range(main_columns)] for _ in range(main_rows)]
        self.small_pins = [[False for _ in range(small_columns)] for _ in range(small_rows)]
        self.last_clicked = None  # Store the last pin clicked
        self.connections = []  # Store the connections
        self.create_widgets()
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.serial_conn = None

        self.initialize_serial()
        self.write_to_serial("Clear")
        self.usedMUX1Pins = []
        self.usedMUX2Pins = []
    
    def initialize_serial(self):
        try:
            self.serial_conn = serial.Serial(self.serial_port, self.baud_rate, timeout=1)
            time.sleep(2)  # give the connection a second to settle
            print("Serial Connection Established.")
            if self.wait_for_arduino():
                print("Arduino is ready.")
        except Exception as e:
            print(f"Failed to connect to Arduino via serial: {e}")

    def wait_for_arduino(self):
        # Wait for the Arduino to send 'Ready'
        while True:
            if self.serial_conn.in_waiting > 0:
                line = self.serial_conn.readline().decode().strip()
                if line == "Ready":
                    return True
            time.sleep(0.1)

    def write_to_serial(self, message):
        if self.serial_conn:
            try:
                self.serial_conn.write(bytes(message, 'utf-8'))
                print(f"Sent to Arduino: {message}")
            except Exception as e:
                print(f"Failed to send message: {e}")

    def create_sidebar(self):
        N_BUTTONS = [
            {"name": "Component 1", "type": "LED"},
            {"name": "Component 2", "type": "LED"},
            {"name": "Component 3", "type": "Resistor"},
            {"name": "Component 4", "type": "Capacitor"},
            {"name": "Component 5", "type": "Switch"},
            {"name": "Component 6", "type": "Transistor"},
            {"name": "Component 7", "type": "Diode"},
            {"name": "Component 8", "type": "Sensor"},
            {"name": "Component 9", "type": "Motor"},
            {"name": "Component 10", "type": "Speaker"}
        ]

        active_buttons_from_search = N_BUTTONS.copy()

        # Create a frame for the sidebar
        sidebar_frame = tk.Frame(self, width=200, height=self.winfo_height(), bg='white')
        sidebar_frame.pack(side=tk.RIGHT, fill=tk.Y)
        sidebar_frame.pack_propagate(0)

        # Create a label for the chatbot
        comp_label = tk.Label(sidebar_frame, text='ChatBot', font=('Arial', 20), bg=colors[6])
        comp_label.pack(side=tk.TOP, fill=tk.X)

        # Create a scrollable frame for the buttons
        scrollable_frame = tk.Frame(sidebar_frame, bg='white')
        scrollable_frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        # Create a scrollbar
        scrollbar = tk.Scrollbar(scrollable_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        # Create a canvas to hold the buttons
        canvas = tk.Canvas(scrollable_frame, bg='white', yscrollcommand=scrollbar.set)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Configure the scrollbar to work with the canvas
        scrollbar.config(command=canvas.yview)

        # Create a frame inside the canvas to hold the buttons
        buttons_frame = tk.Frame(canvas, bg='white')

        # Add the buttons to the buttons frame
        for i in range(len(N_BUTTONS)):
            button = tk.Button(buttons_frame, text=N_BUTTONS[i]["name"], font=('Arial', 12), bg='white')
            button.pack(side=tk.TOP, fill=tk.X, pady=5)
        
        # Center the buttons frame for the X axis
        buttons_frame.update_idletasks()
        canvas_width = canvas.winfo_width()
        buttons_frame_width = buttons_frame.winfo_width()
        x_offset = (canvas_width - buttons_frame_width) // 2
        canvas.create_window((x_offset, 0), window=buttons_frame, anchor='nw')

        # Configure the canvas to update scroll region when the buttons frame changes size
        buttons_frame.bind('<Configure>', lambda event: canvas.configure(scrollregion=canvas.bbox('all')))


        # Create a search component text box
        search_comp = tk.Text(sidebar_frame, bg='black', font=('Arial', 12), height=2, fg='white')
        search_comp.insert(tk.END, "Search component...")
        search_comp.pack(side=tk.BOTTOM)

        # Add placeholder functionality
        def on_entry_click(event):
            if search_comp.get("1.0", "end-1c") == "Search component...":
                search_comp.delete("1.0", tk.END)
                search_comp.config(fg='white')

        def on_focus_out(event):
            if search_comp.get("1.0", "end-1c") == "":
                search_comp.insert(tk.END, "Search component...")
                search_comp.config(fg='white')

        search_comp.bind("<FocusIn>", on_entry_click)
        search_comp.bind("<FocusOut>", on_focus_out)

        # Add white border to the text box
        search_comp.config(borderwidth=2, relief=tk.SOLID, highlightbackground=colors[6])

        # Add search functionality
        def search_components(event):
            search_text = search_comp.get("1.0", "end-1c")
            if search_text != "Search component...":
                active_buttons_from_search.clear()
                for button in N_BUTTONS:
                    if search_text.lower() in button["name"].lower():
                        active_buttons_from_search.append(button)
                update_buttons_frame()

        def update_buttons_frame():
            for widget in buttons_frame.winfo_children():
                widget.destroy()
            for i in range(len(active_buttons_from_search)):
                button = tk.Button(buttons_frame, text=active_buttons_from_search[i]["name"], font=('Arial', 12), bg='white')
                button.pack(side=tk.TOP, fill=tk.X, pady=5)
            
            # Center the buttons
            buttons_frame.update_idletasks()
            sidebar_width = sidebar_frame.winfo_width()
            buttons_width = buttons_frame.winfo_width()
            padding = (sidebar_width - buttons_width) // 2
            buttons_frame.pack_configure(padx=padding)

        search_comp.bind("<KeyRelease>", search_components)

    def create_widgets(self):
        self.main_frame = tk.Frame(self, bd=2, relief=tk.RAISED)
        self.main_frame.pack(side=tk.BOTTOM, padx=10, pady=10)
        self.main_buttons = [[None for _ in range(self.main_columns)] for _ in range(self.main_rows)]
        
        count = 1
        for i in range(self.main_rows):
            for j in range(self.main_columns):
                label_text = str(count)
                self.main_buttons[i][j] = tk.Button(self.main_frame, text=label_text, width=4, height=3, relief="raised",
                                                    command=lambda row=i, column=j: self.toggle_pin(row, column, 'main'))
                self.main_buttons[i][j].grid(row=i, column=j, padx=4, pady=4)
                count += 1

        self.small_frame = tk.Frame(self, bd=2, relief=tk.RAISED)
        self.small_frame.pack(side=tk.TOP, padx=10, pady=10)
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
            button.config(relief="sunken", bg=colors[len(self.connections) % len(colors)])  # Set color to a unique color for this connection
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

        if pin1[0] == 'small' and pin2[0] == 'main':
            MCUNonTuplePin1 = self.get_pin_number(pin1)
            mainNonTuplePin2 = self.get_pin_number(pin2)
        elif pin1[0] == 'main' and pin2[0] == 'small':
            MCUNonTuplePin1 = self.get_pin_number(pin2)
            mainNonTuplePin2 = self.get_pin_number(pin1)

        if MCUNonTuplePin1 == 5:
            MCUNonTuplePin1 = 8
        elif MCUNonTuplePin1 == 6:
            MCUNonTuplePin1 = 7
        elif MCUNonTuplePin1 == 7:
            MCUNonTuplePin1 = 6
        elif MCUNonTuplePin1 == 8:
            MCUNonTuplePin1 = 5

        
    
        # print(f"MCU Pin: {MCUNonTuplePin1}, Main Pin: {mainNonTuplePin2}")
                                                                                # MCU pin, Main pin, mode
        toWriteToCU = export_connections(load_multiplexer_config('rules.json'), MCUNonTuplePin1, mainNonTuplePin2, "true", self.usedMUX1Pins, self.usedMUX2Pins)

        mainLedsPin = mainNonTuplePin2 - 1
        mcuLedsPin = MCUNonTuplePin1 - 1

        if mcuLedsPin == 0:
            mcuLedsPin = 7
        elif mcuLedsPin == 1:
            mcuLedsPin = 6
        elif mcuLedsPin == 2:
            mcuLedsPin = 5
        elif mcuLedsPin == 3:
            mcuLedsPin = 4
        elif mcuLedsPin == 4:
            mcuLedsPin = 0
        elif mcuLedsPin == 5:
            mcuLedsPin = 1
        elif mcuLedsPin == 6:
            mcuLedsPin = 2
        elif mcuLedsPin == 7:
            mcuLedsPin = 3

        ledsString = ";" + "MainBreadboard " + str(mainLedsPin) + ";" + "MCUBreadboard " + str(mcuLedsPin)
        toWriteToCU += ledsString
        toWriteToCU += "\n"

        # print(f"To write in serial: \n{toWriteToCU}")

        self.write_to_serial(toWriteToCU)


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



                if connection[0][0] == 'small' and connection[1][0] == 'main':
                    MCUNonTuplePin1 = self.get_pin_number(connection[0])
                    mainNonTuplePin2 = self.get_pin_number(connection[1])
                elif connection[0][0] == 'main' and connection[1][0] == 'small':
                    MCUNonTuplePin1 = self.get_pin_number(connection[1])
                    mainNonTuplePin2 = self.get_pin_number(connection[0])

                
                if MCUNonTuplePin1 == 5:
                    MCUNonTuplePin1 = 8
                elif MCUNonTuplePin1 == 6:
                    MCUNonTuplePin1 = 7
                elif MCUNonTuplePin1 == 7:
                    MCUNonTuplePin1 = 6
                elif MCUNonTuplePin1 == 8:
                    MCUNonTuplePin1 = 5


                toWriteToCU = export_connections(load_multiplexer_config('rules.json'), MCUNonTuplePin1, mainNonTuplePin2, "false", self.usedMUX1Pins, self.usedMUX2Pins)

                mainLedsPin = mainNonTuplePin2 - 1
                mcuLedsPin = MCUNonTuplePin1 - 1

                # print(f"MainLedsPin: {mainLedsPin}, MCULedsPin: {mcuLedsPin} | BEFORE")                

                if mcuLedsPin == 0:
                    mcuLedsPin = 7
                elif mcuLedsPin == 1:
                    mcuLedsPin = 6
                elif mcuLedsPin == 2:
                    mcuLedsPin = 5
                elif mcuLedsPin == 3:
                    mcuLedsPin = 4
                elif mcuLedsPin == 4:
                    mcuLedsPin = 0
                elif mcuLedsPin == 5:
                    mcuLedsPin = 1
                elif mcuLedsPin == 6:
                    mcuLedsPin = 2
                elif mcuLedsPin == 7:
                    mcuLedsPin = 3

                # print(f"MainLedsPin: {mainLedsPin}, MCULedsPin: {mcuLedsPin}")                

                ledsString = ";" + "MainBreadboard " + str(mainLedsPin) + ";" + "MCUBreadboard " + str(mcuLedsPin)
                toWriteToCU += ledsString
                # split the stringt by "\n" and insert leds string after the first line
                
                # print(f"To write in serial: \n{toWriteToCU}")
                toWriteToCU += "\n"
                self.write_to_serial(toWriteToCU)
                
                # to fix formatting !!!c
                # ser.write(b'1000;y5;x10;true;MainBreadboard 15;MCUBreadboard 8\n') THIS IS VALID
                # NUMBERING OF LEDS STARTS FROM 0
                # y and x lower, remove space after ;, T on true lower
                # leds on mcu breadboard are cooked


    def get_pin_number(self, pin):
        board_type, row, column = pin
        if board_type == 'main':
            return row * self.main_columns + column + 1
        else:
            return (self.small_rows - row - 1) * self.small_columns + column + 1

if __name__ == "__main__":
    root = Breadboard(main_rows=2, main_columns=12, small_rows=2, small_columns=4, serial_port='COM3')
    root.mainloop()
