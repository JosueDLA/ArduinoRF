from tkinter import ttk
from tkinter import filedialog as fd
from tkinter import *
from socket import *
import threading
import serial
import time
import re

oldMsg = "lol"

class Ventana(Frame):
	def __init__(self, master):
		"""Inicializar Frame"""
		super().__init__(master)
		master.title("Receptor")

		

		self.grid()
		self.crearWidgets()
		self.grid(padx=20, pady=20)

		daemon = threading.Thread(target = self.mensajes, daemon=True)
		daemon.start()

	def mensajes(self):
		while True:
			soc = socket(AF_INET, SOCK_STREAM)
			soc.connect(('192.168.1.69', 80))
			soc.send('GET \n\n'.encode('UTF-8')) # Note the double \n\n at the end.

			response = soc.recv(500)

			msg = response.decode('UTF-8')

			global oldMsg

			if(oldMsg != msg):
				user = self.combo1.get()

				print(msg)
				mensaje = re.split("\~", msg, 1)

				mensaje[0] = re.sub("@", "", mensaje[0])
				mensaje[1] = re.sub("~", "", mensaje[1])

				if(user == mensaje[0]):
					print(user)
					print(mensaje[0])
					self.text1.insert(END, mensaje[1])
					print(mensaje)

				oldMsg = msg
			time.sleep(2)




	def crearWidgets(self):
		"""Creacion de todos los widgets a utilizar"""

		self.usuario = ["User1", "User2", "User3", "User4", "User5"]
		
		Label(self, text =  " ").grid(column=0, row=1)
		Label(self, text =  "Seleccione un usuario: ").grid(column=0, row=2, sticky=W)

		self.combo1 = ttk.Combobox(self, state="readonly", width=37)
		self.combo1["values"] = self.usuario
		self.combo1.set(self.usuario[0])
		self.combo1.grid(column=0, row=3, sticky=W)

		Label(self, text =  " ").grid(column=0, row=4)

		Label(self, text =  "Inbox:").grid(column=0, row=5, sticky=W)

		self.text1 = Text(self,height=10,width=30)
		
		scroll = Scrollbar(self, orient='vertical')
		
		self.text1.configure(yscrollcommand=scroll.set)
		self.text1.grid(column=0, row=6, sticky=W)
		
		scroll.grid(column=6, row=6, sticky=E)

root = Tk()
root.geometry("300x350")
root.resizable(False, False)
app = Ventana(root)
app.mainloop()