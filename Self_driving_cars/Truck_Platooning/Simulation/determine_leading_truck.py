"""A python file that determins the leading truck using machine learning"""


import tkinter as tk
from tkinter import END, ttk
from PIL import ImageTk, Image

import numpy as np
import pandas as pd

from sklearn.tree import DecisionTreeClassifier   
from sklearn.model_selection import train_test_split  

import pygame
import time
import math
from utils import scale_image, blit_rotate_center





App = tk.Tk()
#set the title
App.title=("Leading truck determination")

drf = ttk.Frame(App)
drf.grid(padx=10, sticky=(tk.E + tk.W))
drf.columnconfigure(0, weight=1)


###########################################################################################################################
"Truck 1"
truck_1_section = ttk.LabelFrame(drf, text="Truck 1")
truck_1_section.grid(sticky=(tk.W+tk.E))
for i in range(5):
    truck_1_section.columnconfigure(i, weight=1)

"Truck size"
t1_size_label = ttk.Label(truck_1_section,text="Size of truck").grid(row=0,column=0)
t1_size_storage = tk.IntVar()
t1_size_entry = ttk.Entry(truck_1_section,textvariable=t1_size_storage) #I did  not type .grid() here. Because there is a reset button, and for some reason adding .grid() makes the Entry a none type, and you can do nothing with None type
t1_size_entry.grid(row=0,column=1)
t1_size_unit = ttk.Label(truck_1_section,text="(feet)").grid(row=0, column=2)

"Distance to be covered"
t1_distance_label = ttk.Label(truck_1_section,text="The distance to be travelled").grid(row=1,column=0)
t1_distance_storage = tk.IntVar()
t1_distance_entry = ttk.Entry(truck_1_section,textvariable=t1_distance_storage)
t1_distance_entry.grid(row=1,column=1)
t1_diatance_unit = ttk.Label(truck_1_section,text="(Km)").grid(row=1, column=2)

"Break speed"
t1_break_speed_label = ttk.Label(truck_1_section,text="Truck break speed").grid(row=2,column=0)
t1_break_speed_storage = tk.DoubleVar()
t1_break_speed_entry = ttk.Entry(truck_1_section,textvariable=t1_break_speed_storage)
t1_break_speed_entry.grid(row=2,column=1)
t1_break_unit = ttk.Label(truck_1_section,text="(s)").grid(row=2, column=2)

"Energy consumption"
t1_energy_consumption_label = ttk.Label(truck_1_section,text="Truck energy_consumption").grid(row=3,column=0)
t1_energy_consumption_storage = tk.DoubleVar()
t1_energy_consumption_entry = ttk.Entry(truck_1_section,textvariable=t1_energy_consumption_storage)
t1_energy_consumption_entry.grid(row=3,column=1)
t1_energy_unit = ttk.Label(truck_1_section,text="(Wh/Km)").grid(row=3, column=2)

"Number of sensors"
t1_number_of_sensors_label = ttk.Label(truck_1_section,text="Number of sensors on truck").grid(row=4,column=0)
t1_number_of_sensors_storage = tk.IntVar()
t1_number_of_sensors_entry = ttk.Entry(truck_1_section,textvariable=t1_number_of_sensors_storage)
t1_number_of_sensors_entry.grid(row=4,column=1)
t1_number_of_sensors_unit = ttk.Label(truck_1_section,text="out of 5").grid(row=4, column=2)



###################################################################################################################################################################################################





###########################################################################################################################
"Truck 2"
truck_2_section = ttk.LabelFrame(drf, text="Truck 2")
truck_2_section.grid(sticky=(tk.W+tk.E))
for i in range(5):
    truck_1_section.columnconfigure(i, weight=1)

"Truck size"
t2_size_label = ttk.Label(truck_2_section,text="Size of truck").grid(row=0,column=0)
t2_size_storage = tk.IntVar()
t2_size_entry = ttk.Entry(truck_2_section,textvariable=t2_size_storage)
t2_size_entry.grid(row=0,column=1)
t2_size_unit = ttk.Label(truck_2_section,text="(feet)").grid(row=0, column=2)

"Distance to be covered"
t1_distance_label = ttk.Label(truck_2_section,text="The distance to be travelled").grid(row=1,column=0)
t2_distance_storage = tk.IntVar()
t2_distance_entry = ttk.Entry(truck_2_section,textvariable=t2_distance_storage)
t2_distance_entry.grid(row=1,column=1)
t2_distance_unit = ttk.Label(truck_2_section,text="(Km)").grid(row=1, column=2)

"Break speed"
t2_break_speed_label = ttk.Label(truck_2_section,text="Truck break speed").grid(row=2,column=0)
t2_break_speed_storage = tk.DoubleVar()
t2_break_speed_entry = ttk.Entry(truck_2_section,textvariable=t2_break_speed_storage)
t2_break_speed_entry.grid(row=2,column=1)
t2_break_unit = ttk.Label(truck_2_section,text="(s)").grid(row=2, column=2)

"Energy consumption"
t2_energy_consumption_label = ttk.Label(truck_2_section,text="Truck energy_consumption").grid(row=3,column=0)
t2_energy_consumption_storage = tk.DoubleVar()
t2_energy_consumption_entry = ttk.Entry(truck_2_section,textvariable=t2_energy_consumption_storage)
t2_energy_consumption_entry.grid(row=3,column=1)
t2_energy_consumption_unit = ttk.Label(truck_2_section,text="(Wh/Km)").grid(row=3, column=2)

"Number of sensors"
t2_number_of_sensors_label = ttk.Label(truck_2_section,text="Number of sensors on truck").grid(row=4,column=0)
t2_number_of_sensors_storage = tk.IntVar()
t2_number_of_sensors_entry = ttk.Entry(truck_2_section,textvariable=t2_number_of_sensors_storage)
t2_number_of_sensors_entry.grid(row=4,column=1)
t2_number_of_sensors_unit = ttk.Label(truck_2_section,text="out of 5").grid(row=4, column=2)



###################################################################################################################################################################################################




###########################################################################################################################
"Truck 3"
truck_3_section = ttk.LabelFrame(drf, text="Truck 3")
truck_3_section.grid(sticky=(tk.W+tk.E))
for i in range(5):
    truck_1_section.columnconfigure(i, weight=1)

"Truck size"
t3_size_label = ttk.Label(truck_3_section,text="Size of truck").grid(row=0,column=0)
t3_size_storage = tk.IntVar()
t3_size_entry = ttk.Entry(truck_3_section,textvariable=t3_size_storage)
t3_size_entry.grid(row=0,column=1)
t3_size_unit = ttk.Label(truck_3_section,text="(feet)").grid(row=0, column=2)

"Distance to be covered"
t3_distance_label = ttk.Label(truck_3_section,text="The distance to be travelled").grid(row=1,column=0)
t3_distance_storage = tk.IntVar()
t3_distance_entry = ttk.Entry(truck_3_section,textvariable=t3_distance_storage)
t3_distance_entry.grid(row=1,column=1)
t3_distance_unit = ttk.Label(truck_3_section,text="(Km)").grid(row=1, column=2)

"Break speed"
t3_break_speed_label = ttk.Label(truck_3_section,text="Truck break speed").grid(row=2,column=0)
t3_break_speed_storage = tk.DoubleVar()
t3_break_speed_entry = ttk.Entry(truck_3_section,textvariable=t3_break_speed_storage)
t3_break_speed_entry.grid(row=2,column=1)
t3_break_speed_unit = ttk.Label(truck_3_section,text="(s)").grid(row=2, column=2)

"Energy consumption"
t3_energy_consumption_label = ttk.Label(truck_3_section,text="Truck energy_consumption").grid(row=3,column=0)
t3_energy_consumption_storage = tk.DoubleVar()
t3_energy_consumption_entry = ttk.Entry(truck_3_section,textvariable=t3_energy_consumption_storage)
t3_energy_consumption_entry.grid(row=3,column=1)
t3_energy_consumption_unit = ttk.Label(truck_3_section,text="(Wh/Km)").grid(row=3, column=2)

"Number of sensors"
t3_number_of_sensors_label = ttk.Label(truck_3_section,text="Number of sensors on truck").grid(row=4,column=0)
t3_number_of_sensors_storage = tk.IntVar()
t3_number_of_sensors_entry = ttk.Entry(truck_3_section,textvariable=t3_number_of_sensors_storage)
t3_number_of_sensors_entry.grid(row=4,column=1)
t3_number_of_sensors_unit = ttk.Label(truck_3_section,text="out of 5").grid(row=4, column=2)



###################################################################################################################################################################################################





#############################################################################################################################################################################
"""This is storage variable for the output (leading truck) is created"""
message_section = ttk.LabelFrame(drf, text="Results") #used inside the on_determine_leading_truck_button function

output_var = tk.StringVar(value=" ")
output_line = tk.Label(App,textvariable=output_var,anchor="w",
    justify="left").grid(row=100,columnspan=2,sticky="NSEW")

################################################################################################################################################################################

buttons = tk.Frame(drf)
buttons.grid(sticky=tk.E + tk.W)

def on_reset():
    "Truck_1"
    t1_size_entry.delete(0,END)
    t1_distance_entry.delete(0,END)
    t1_break_speed_entry.delete(0,END)
    t1_energy_consumption_entry.delete(0,END)
    t1_number_of_sensors_entry.delete(0,END)

    "Truck_2"
    t2_size_entry.delete(0,END)
    t2_distance_entry.delete(0,END)
    t2_break_speed_entry.delete(0,END)
    t2_energy_consumption_entry.delete(0,END)
    t2_number_of_sensors_entry.delete(0,END)

    "Truck_3"
    t3_size_entry.delete(0,END)
    t3_distance_entry.delete(0,END)
    t3_break_speed_entry.delete(0,END)
    t3_energy_consumption_entry.delete(0,END)
    t3_number_of_sensors_entry.delete(0,END)


def on_determine_leading_truck_button():
    ##I load the truck data set created
    data = pd.read_csv("truck_data_set.csv")
    features = ["Size(feet)","Distance_to_be_covered(Km)","Break_Speed(s)","Energy_Consumption(Wh/km)","Sensors"]
    X = data[features]
    y = data.Leading_Truck
    X_train, X_test, y_train, y_test = train_test_split(X,y,test_size=0.3,random_state=1)
    DT = DecisionTreeClassifier()
    DT_train = DT.fit(X_train,y_train)
    y_predict = DT.predict(X_test)

    #Leading Truck prediction section
    truck1_user_input = [[int(t1_size_storage.get()),int(t1_distance_storage.get()),float(t1_break_speed_storage.get()),float(t1_energy_consumption_storage.get()),int(t1_number_of_sensors_storage.get())]]
    truck2_user_input = [[int(t2_size_storage.get()),int(t2_distance_storage.get()),float(t2_break_speed_storage.get()),float(t2_energy_consumption_storage.get()),int(t2_number_of_sensors_storage.get())]]
    truck3_user_input = [[int(t3_size_storage.get()),int(t3_distance_storage.get()),float(t3_break_speed_storage.get()),float(t3_energy_consumption_storage.get()),int(t3_number_of_sensors_storage.get())]]

    truck1_predicted_value = DT.predict(truck1_user_input)
    truck2_predicted_value = DT.predict(truck2_user_input)
    truck3_predicted_value = DT.predict(truck3_user_input)
    
    if (truck1_predicted_value ==1):
        message = "Truck 1 can be the leading truck"
        output_var.set(message)
        print(message)
    else:
        message = "Truck 1 can not be the leading truck"
        output_var.set(message)
        print(message)

    if (truck2_predicted_value ==1):
        message = "Truck 1 can be the leading truck"
        output_var.set(message)
        print(message)

    if (truck3_predicted_value ==1):
        message = "Truck 1 can be the leading truck"
        output_var.set(message)
        print(message)



def on_start_simulation_button():
    pass
    
    

    


   

reset_button = ttk.Button(buttons, text="Reset")
reset_button.pack(side=tk.RIGHT)
reset_button.configure(command=on_reset)

determine_leading_truck_button = ttk.Button(buttons, text="Determine Leading Truck")
determine_leading_truck_button.pack(side=tk.RIGHT)
determine_leading_truck_button.configure(command=on_determine_leading_truck_button)

start_simulation_button = ttk.Button(buttons, text="Start simulation")
start_simulation_button.pack(side=tk.RIGHT)
start_simulation_button.configure(command=on_start_simulation_button)





"""
# Create an object of tkinter ImageTk
img = ImageTk.PhotoImage(Image.open("Leading_truck.png"))

image_section = ttk.LabelFrame(drf, text="Decision Tree image")
image_section.grid(sticky=(tk.W+tk.E))
#for i in range(5):
#    image_section.columnconfigure(i, weight=1)


# Create a Label Widget to display the text or Image
label = ttk.Label(image_section, image = img)
label.pack()


"""

App.mainloop()

