import tkinter as tk
from tkinter import messagebox

from Serial import Serial

class UI(tk.Tk):
    def __init__(self, dimension, title, background_color):
        super().__init__()
        self.ser = Serial()
        if dimension == "full-screen":
            screen_width = self.winfo_screenwidth()
            screen_height = self.winfo_screenheight()
            self.width = screen_width
            self.height = screen_height - 80
        else:
            dimension_splited = dimension.split("x")
            self.width = int(dimension_splited[0])
            self.height = int(dimension_splited[1])
        self.title_app = title
        self.resizable(False, False)
        self["bg"] = background_color
        self.create_window()
        self.create_com_control()
        self.create_key_control()
        self.create_offset_control()
        self.create_effect_control()
        # self.create_control_panel() ## control_panel
        # self.create_graph() # graph

    def create_window(self):
        """mettre la dimension de la titre"""
        dimension_str = str(self.width) + "x" + str(self.height) + "+-10+0"
        self.geometry(dimension_str)
        self.title(self.title_app)

    def create_com_control(self):
        self.frame_com = tk.LabelFrame(self, text="Com Manager", bg=self["bg"], font=("MonoLisa", 13))
        self.frame_com.pack()
        self.refresh_btn = tk.Button(self.frame_com, text="Refresh", width=10, command=self.refresh_cmd, bg="#04aa6d", activebackground='#e7700d', fg="white", activeforeground="white")
        self.connect_btn = tk.Button(self.frame_com, text="Connect", width=10, state="disabled", command=self.connect_cmd, bg="#04aa6d", activebackground='#e7700d', fg="white", activeforeground="white", disabledforeground="red")
        self.com_widget()
        bds = ["-", "9600", "115200"]
        self.clicked_bd = tk.StringVar()
        self.clicked_bd.set(bds[0])
        self.drop_bd = tk.OptionMenu(self.frame_com, self.clicked_bd, *bds, command=self.com_ctrl)
        self.drop_bd.config(width=10)

        self.label_com = tk.Label(self.frame_com, text="Available port(s): ", bg=self["bg"], width=15, anchor="w")
        self.label_bd = tk.Label(self.frame_com, text="Baude rate: ", bg=self["bg"], width=15, anchor="w")
        # self.frame_com.grid(row=0, column=3, columnspan=2, sticky="nsew", pady=(0, 50))
        self.label_com.grid(row=0, column=0)
        self.drop_com.grid(row=0, column=1)
        self.label_bd.grid(row=1, column=0)
        self.drop_bd.grid(row=1, column=1)
        self.refresh_btn.grid(row=0, column=3, padx=10)
        self.connect_btn.grid(row=1, column=3, padx=10)

    def create_key_control(self):
        self.frame_key = tk.LabelFrame(self, text="Key Manager", bg=self["bg"], font=("MonoLisa", 13))
        self.frame_key.pack()


        self.key = tk.StringVar()
        self.key.set("C")
        self.ser.write("C")
        self.C_button = tk.Radiobutton(self.frame_key, text="C", variable=self.key, value="C", background=self["bg"], command=self.change_key)
        self.D_button = tk.Radiobutton(self.frame_key, text="D", variable=self.key, value="D", background=self["bg"], command=self.change_key)
        self.E_button = tk.Radiobutton(self.frame_key, text="E", variable=self.key, value="E", background=self["bg"], command=self.change_key)
        self.F_button = tk.Radiobutton(self.frame_key, text="F", variable=self.key, value="F", background=self["bg"], command=self.change_key)
        self.G_button = tk.Radiobutton(self.frame_key, text="G", variable=self.key, value="G", background=self["bg"], command=self.change_key)
        self.A_button = tk.Radiobutton(self.frame_key, text="A", variable=self.key, value="A", background=self["bg"], command=self.change_key)
        self.B_button = tk.Radiobutton(self.frame_key, text="B", variable=self.key, value="B", background=self["bg"], command=self.change_key)
        self.C_button.pack()
        self.D_button.pack()
        self.E_button.pack()
        self.F_button.pack()
        self.G_button.pack()
        self.A_button.pack()
        self.B_button.pack()

    def create_offset_control(self):
        self.offset = tk.IntVar()
        self.frame_offset = tk.LabelFrame(self, text="Offset Manager", bg=self["bg"], font=("MonoLisa", 13))
        self.frame_offset.pack()
        self.offset_scale = tk.Scale(self.frame_offset, variable=self.offset, from_=-24, to=48, orient="horizontal", troughcolor="#04aa6d", command=self.change_offset)
        self.offset_scale.pack()
        self.offset_entry = tk.Entry(self.frame_offset, textvariable=str(self.offset))
        self.offset_entry.pack()

    def create_effect_control(self):
        self.frame_effect = tk.LabelFrame(self, text="Effect Manager", bg=self["bg"], font=("MonoLisa", 13))
        self.frame_effect.pack()
        self.on_echo = tk.IntVar()
        self.echo_checkbutton = tk.Checkbutton(self.frame_effect, text="Echo", variable=self.on_echo, onvalue = 1, offvalue = 0, bg=self["bg"], command=self.change_echo)
        self.echo_checkbutton.pack()


    def com_widget(self):
        """afficher les ports disponibles"""
        self.ser.get_ports()
        self.clicked_com = tk.StringVar()
        self.clicked_com.set(self.ser.ports[0])
        self.clicked_com.set("-")
        self.drop_com = tk.OptionMenu(self.frame_com, self.clicked_com, *self.ser.ports, command=self.com_ctrl)
        self.drop_com.config(width=10)

    def com_ctrl(self):
        """activer le connect button quand on saisit le port et le baudrate"""
        if "-" in self.clicked_com.get() or "-" in self.clicked_bd.get():
            pass
        else: # tous saisir
            self.connect_btn["state"] = "normal"

    def refresh_cmd(self):
        """chercher le port et le baudrate"""
        print("refresh")
        self.com_widget() # get ports
        self.drop_com.grid(row=0, column=1) # afficher

    def connect_cmd(self):
        """"connect arduino en appuyant le connect button"""
        print("connect")
        if self.connect_btn["text"] in "Connect":
            self.ser.SerialOpen(self) ## essayer d'ouvrir la porte
            if self.ser.ser.status:
                self.connect_btn["text"] = "Disconnect"
                self.refresh_btn["state"] = "disable"
                self.drop_bd["state"] = "disable"
                self.drop_com["state"] = "disable"
                InfoMsg = f"Successfully connect to {self.clicked_com.get()}"
                messagebox.showinfo("showinfo", InfoMsg)
            else:
                ErrorMsg = f"Failure to establish UART connection using {self.clicked_com.get()}"
                messagebox.showerror("showerror", ErrorMsg)
        else:
            self.ser.SerialClose()
            WarnMsg = f"UART connection using {self.clicked_com.get()} is now closed"
            messagebox.showwarning("showinfo", WarnMsg)
            self.connect_btn["text"] = "Connect"
            self.refresh_btn["state"] = "active"
            self.drop_bd["state"] = "active"
            self.drop_com["state"] = "active"

    def change_key(self):
        print(self.key.get())
        self.ser.write(self.key.get())

    def change_offset(self, value):
        print(self.offset.get())
        print("value",value)

    def change_echo(self):
        print(self.on_echo.get())