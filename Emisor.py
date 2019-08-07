from tkinter import ttk
from tkinter import filedialog as fd
from tkinter import *
import serial
import time

class Ventana(Frame):
	def __init__(self, master):
		"""Inicializar Frame"""
		super().__init__(master)
		master.title("Proyecto Final IA")

		self.grid()
		self.crearWidgets()
		self.grid(padx=20, pady=20)


	def crearWidgets(self):
		"""Creacion de todos los widgets a utilizar"""

		self.carros = IntVar()
		self.peatones = IntVar()
		self.motos = IntVar()
		self.buses = IntVar()

		self.usuario = ["User1", "User2", "User3", "User4", "User5"]
		
		Label(self, text =  " ").grid(column=0, row=1)
		Label(self, text =  "Seleccione un usuario: ").grid(column=0, row=2, sticky=W)

		self.combo1 = ttk.Combobox(self, state="readonly", width=37)
		self.combo1["values"] = self.usuario
		self.combo1.set(self.usuario[0])
		self.combo1.grid(column=0, row=3, sticky=W)

		Label(self, text =  " ").grid(column=0, row=4)

		Label(self, text =  "Redacte su mensaje:").grid(column=0, row=5, sticky=W)

		self.text1 = Entry(self, width=40, text="40")
		self.text1.insert(0, "")
		self.text1.grid(column=0, row=6, sticky=W)

		self.boton1 = Button(self, text="Enviar", command=self.enviarMensaje)
		self.boton1.grid(column=0, row=7, padx=10, pady=10)

	def enviarMensaje(self):
		try:
			ser = serial.Serial()
			ser.baudrate = 9600
			ser.port = 'COM3'
			ser.open()
			print(ser.readline().decode('UTF-8'))

			user = self.combo1.get()
			message = self.text1.get()

			fullMessage = "@"+user+"~"+message+"~ "

			ser.write(fullMessage.encode('UTF-8'))
			time.sleep(0.1)
			ser.close()

			self.text1.delete(0, END)

		except serial.SerialException:
			print("Error")
			pass

root = Tk()
root.geometry("300x250")
root.resizable(False, False)
app = Ventana(root)
app.mainloop()