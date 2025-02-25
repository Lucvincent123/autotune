# import tkinter as tk

import config as c


from UI import UI



class App:
    def __init__(self):
        self.ui = UI(c.DIMENSION, c.TITLE, c.BACKGROUND_COLOR)

    def start(self):
        self.ui.mainloop()


if __name__ == "__main__":
    App().start()

