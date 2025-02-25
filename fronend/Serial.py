import serial.tools.list_ports

class Serial:
    def __init__(self) -> None:
        self.ports = []
        self.ser = None

    def get_ports(self):
        """afficher les ports d'arduino disponibles"""        
        ports = serial.tools.list_ports.comports() 
        self.ports = [port[0] for port in ports]
        self.ports.insert(0, "-")

    def SerialOpen(self, gui):
        """Connecter arduino, une fenêtre apparait pour dire si on réussit ou pas"""
        try:
            self.ser.is_open #vérifier s'il y a un port ouvert
        except:
            PORT = gui.clicked_com.get()
            BAUD = gui.clicked_bd.get()
            self.ser = serial.Serial(port=PORT, baudrate=BAUD, timeout=0.1)

        try:
            if self.ser.is_open:
                self.ser.status = True
            else:
                PORT = gui.clicked_com.get()
                BAUD = gui.clicked_bd.get()
                self.ser = serial.Serial(port=PORT, baudrate=BAUD, timeout=0.1)
                self.ser.open()
                self.ser.status = True
        except:
            self.ser.status = False

    def SerialClose(self):
        """Fermer la connection"""
        try:
            self.ser.is_open
            self.ser.close()
            self.ser.status = False
        except:
            self.ser.status = False

    def write(self, msg):
        """envoyer une commande (en bytes) à arduino"""
        if self.ser != None:
            bytes_msg = bytes(msg, "utf-8") #convertir la message de string à byte
            self.ser.write(bytes_msg)

    def readline(self):
        """lire les valeurs envoyées par arduino"""
        if self.ser != None:
            bytes_msg = self.ser.read_until(b"\n")
            return bytes_msg.decode("utf-8") #convertir les valeurs en string