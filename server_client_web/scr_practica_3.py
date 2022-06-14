from functools import partial
from socketIO_client import SocketIO
from tkinter import *
import speech_recognition as sr

id = "20211961"
print("Comenzando...")
socketIO = SocketIO('localhost', 5001)
print("Conectado al servidor.")

led_on = ["LIGHT", "ON"]
led_encender = ["LUZ","ENCENDER"]
led_prender = ["LUZ","PRENDER"]
led_off = ["LIGHT", "OFF"]
led_apagar = ["LUZ", "APAGAR"]

temperature_on = ["TEMPERATURE", "ON"]
temperature_encender = ["TEMPERATURA","ENCENDER"]
temperature_prender = ["TEMPERATURA","PRENDER"]
temperature_off = ["TEMPERATURE", "OFF"]
temperature_apagar = ["TEMPERATURA","APAGAR"]

humidity_on = ["HUMIDITY", "ON"]
humidity_encender = ["HUMEDAD","ENCENDER"]
humidity_prender = ["HUMEDAD","PRENDER"]
humidity_off = ["HUMIDITY", "OFF"]
humidity_apagar = ["HUMEDAD","APAGAR"]

r = sr.Recognizer()
# Micrófono por defecto
mic = sr.Microphone()

class Window(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)        
        self.master = master
        # widget can take all window
        self.pack(fill=BOTH, expand=1)

        entry = Entry(takefocus=True)
        entry.place(x=75,y=5)
        entry.insert(0,'20211961')
        entry.focus()

        tem_on_Button = Button(self, text="TEMPERATURA ON", width=15, command=partial(self.temp_on,entry))
        tem_on_Button.place(x=25, y=30)
        tem_off_Button = Button(self, text="TEMPERATURA OFF", width=15, command=partial(self.temp_off,entry))
        tem_off_Button.place(x=150, y=30)
        tem_on_Button = Button(self, text="HUMEDAD ON", width=15, command=partial(self.hum_on,entry))
        tem_on_Button.place(x=25, y=60)
        tem_off_Button = Button(self, text="HUMEDAD OFF", width=15, command=partial(self.hum_off,entry))
        tem_off_Button.place(x=150, y=60)
        tem_on_Button = Button(self, text="LUZ ON", width=15, command=partial(self.luz_on,entry))
        tem_on_Button.place(x=25, y=90)
        tem_off_Button = Button(self, text="LUZ OFF", width=15, command=partial(self.luz_off,entry))
        tem_off_Button.place(x=150, y=90)

        lbl_audio = Label(root, text="Voz...")
        lbl_audio.pack() #Si hacemos .pack() en la misma línea nos retorna un null, entonces no se puede pasar como argumento

        exitButton = Button(self, text="Enviar comando de voz", width=20, command=partial(self.voice_command,lbl_audio))
        exitButton.place(x=75, y=120)

    def temp_on(self,entry):
        id = entry.get()
        message = {'id':id,'command':"TEMPERATURE_ON"}
        print(message)
        socketIO.emit("controlar_ESP", message)
    def temp_off(self,entry):
        id = entry.get()
        message = {'id':id,'command':"TEMPERATURE_OFF"}
        print(message)
        socketIO.emit("controlar_ESP", message)
    def hum_on(self,entry):
        id = entry.get()
        message = {'id':id,'command':"HUMIDITY_ON"}
        print(message)
        socketIO.emit("controlar_ESP", message)
    def hum_off(self,entry):
        id = entry.get()
        message = {'id':id,'command':"HUMIDITY_OFF"}
        print(message)
        socketIO.emit("controlar_ESP", message)
    def luz_on(self,entry):
        id = entry.get()
        message = {'id':id,'command':"LIGHT_ON"}
        print(message)
        socketIO.emit("controlar_ESP", message)
    def luz_off(self,entry):
        id = entry.get()
        message = {'id':id,'command':"LIGHT_OFF"}
        print(message)
        socketIO.emit("controlar_ESP", message)

    def voice_command(self,lbl_audio):
        # exit()
        with mic as source:
            r.adjust_for_ambient_noise(source)
            #print("Grabando...")
            audio = r.listen(source)
            #print("Traduciendo...")
            text = r.recognize_google(audio)
            text = text.upper()
            #print(text)
            lbl_audio.config(text=text)
            
            if all(x in text for x in led_on) or all(x in text for x in led_encender) or all(x in text for x in led_prender): #any or all
                command = "LIGHT_ON"
            elif all(x in text for x in led_off) or all(x in text for x in led_apagar): #any or all
                command = "LIGHT_OFF"
            elif all(x in text for x in temperature_on) or all(x in text for x in temperature_encender) or all(x in text for x in temperature_prender):
                command = "TEMPERATURE_ON"
            elif all(x in text for x in temperature_off) or all(x in text for x in temperature_apagar):
                command = "TEMPERATURE_OFF"
            elif all(x in text for x in humidity_on) or all(x in text for x in humidity_encender) or all(x in text for x in humidity_prender):
                command = "HUMIDITY_ON"
            elif all(x in text for x in humidity_off) or all(x in text for x in humidity_apagar):
                command = "HUMIDITY_OFF"
            else:
                command = ""
            message = {'id':id,'command':command}
            #print(message)
            #lbl_command.config(text=message)
            socketIO.emit("controlar_ESP", message)
            
root = Tk()
app = Window(root)
root.wm_title("CONTROL")
root.geometry("320x200+700+400")
root.mainloop()