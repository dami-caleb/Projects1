from time import sleep                      #needed to sleep (wait for a specific amount of time)
from threading import Thread, Lock                #needed to carry out out differnt operations at the same time

#LT = Leading Truck 
#T1 =  Following truck 1
#T2 = Following Truck2



run = True

#############################################################################################################################################################################################
# I create a lock instance of the "LOCK()" class and use it to lock the shared variable to avoid race conditions
lock = Lock()

#############################################################################################################################################################################################




#############################################################################################################################################################################################
#Leading Truck

class LeadingTruckStates:
    #Leading Truck state definitions
    idle_state = "The leading truck is idle"
    leading_truck_start_state = "The leading trucks engine is on"
    spacing_confirmation_state = "The leading truck has sent the set space command to the following truck1, and following truck1 has confirmed it has given the space"
    synchronization_state = "The leading truck is synchronizing with the following truck"
    truck1_connected_state = "The leading truck has sucessfully synchronized with following truck1"
    movement_state = "The leading truck is moving"
    ready_to_send_other_control_information_state = "The leading truck is sending the control information to following truck1"

#Leading truck gaurds and conditions definitions

set_spacing = False
LT_control_information = False

#The initial sate of the leading truck 
LT =  LeadingTruckStates.idle_state
#############################################################################################################################################################################################



#############################################################################################################################################################################################
#Following Truck1

class FollowingTruck1States:
    #Following Truck1 state definitions
    idle_state = "Following truck1 is idle"
    spaced_state = "Following truck1 has spaced itself from the leading truck"
    synchronise_with_leader_state = "Following truck1 is synchronising with the leading truck"
    confirmed_synchronization_with_leading_truck_state = "Following truck1 has synchronised with the leading truck"
    space_confirmation_state = "Following truck1 has sent the set space command to the following truck2"
    t2_space_confirmation = "Following truck2 has confirmed it has given the required space to truck1"
    synchronize_with_truck2_state = "Following truck1 has sucessfully synchronized with following truck2"
    movement_state = "Following Truck1 is moving "
    ready_to_receive_other_control_information_state = "Following Truck1 is ready to receive another control information from the leading truck"



#Following Truck1 gaurds and conditions definitions

spacing_set = False
synchronized = False
connection_checker = 0
t2_set_spacing = False
LT_control_information1 = False


#The initial state of following truck1
T1 = FollowingTruck1States.idle_state 
#############################################################################################################################################################################################



#############################################################################################################################################################################################
#Following Truck2

class FollowingTruck2States:
    #Following Truck2 state definitions
    idle_state = "Following truck2 is idle"
    t2_spaced = "Following truck2 has spaced itself from the following truck1"
    synchronise_with_truck1_state =  "Following truck2 has synchronised with the folowing truck1"
    movement_state = "Following Truck2 is moving "
    ready_to_receive_other_control_information_state = "Following Truck2 is ready to receive another control information from following truck1"


#Following Truck2 gaurds and conditions
t2_spacing_set  = False
truck2_connected  = False
T2_carried_out_control_information  = False

#The initial state of following truck2
T2 = FollowingTruck2States.idle_state
#############################################################################################################################################################################################




#############################################################################################################################################################################################
#Control algorithim section

def LEADINGTRUCK():
    while True:
            global LT
            match LT:
                case LeadingTruckStates.idle_state:
                    print(LeadingTruckStates.idle_state)
                    sleep(2) #the sleep is added to make the text human readable
                    if run:
                        LT = LeadingTruckStates.leading_truck_start_state
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable
                
                case LeadingTruckStates.leading_truck_start_state:
                    global set_spacing  #I did added this so that python knows I am talking about the global variable
                    
                    lock.acquire()   #mutex
                    set_spacing = True #mutex here to prevent race conditions
                    lock.release()

                    LT = LeadingTruckStates.spacing_confirmation_state
                    print(LT)
                    sleep(2) #the sleep is added to make the text human readable

                case LeadingTruckStates.spacing_confirmation_state:
                    global spacing_set
                    if spacing_set:
                        LT = LeadingTruckStates.synchronization_state
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable

                        lock.acquire()  #The global variable spacing_set has already been decleared global above #mutex
                        spacing_set = False
                        lock.release()
                
                case LeadingTruckStates.synchronization_state:
                    global synchronized
                    while (connection_checker!=1):
                        LT = LeadingTruckStates.synchronization_state
                        print("Connecting to following truck 1................")
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable
                    if synchronized:
                        LT = LeadingTruckStates.truck1_connected_state
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable
                        

                        lock.acquire() #mutex
                        synchronized = False
                        lock.release()

                case LeadingTruckStates.truck1_connected_state:
                    global truck2_connected # mutex
                    if truck2_connected:
                        LT = LeadingTruckStates.movement_state
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable
                        
                        lock.acquire() #mutex
                        truck2_connected = False
                        lock.release()

                
                case LeadingTruckStates.movement_state:
                    global LT_control_information  #I did added this so that python knows I am talking about the global variable
                    
                    lock.acquire()  #mutex
                    LT_control_information =True
                    lock.release()
                    
                    LT =  LeadingTruckStates.ready_to_send_other_control_information_state
                    print(LT)
                    sleep(2) #the sleep is added to make the text human readable

                case LeadingTruckStates.ready_to_send_other_control_information_state:
                    global T2_carried_out_control_information
                    if T2_carried_out_control_information:
                        LT =LeadingTruckStates.movement_state
                        print(LT)
                        sleep(2) #the sleep is added to make the text human readable
                        
                        lock.acquire()
                        T2_carried_out_control_information = False
                        lock.release()


def FOLLOWINGTRUCK1():
    while True:
        global T1
        match T1:
            case FollowingTruck1States.idle_state:
                global  LT_control_information  # to avoid racing conditions we have to lock the global variable we are about to change
                global set_spacing  #we are updating a global variable so we have to lock it to avoid racing condition
                if set_spacing:
                    T1 = FollowingTruck1States.spaced_state
                    print(T1)
                    sleep(2) #the sleep is added to make the text human readable
                   
                    
                    lock.acquire() #mutex
                    set_spacing = False
                    lock.release()
                
                elif LT_control_information:
                    T1 = FollowingTruck1States.movement_state
                    print(T1)
                    sleep(2) #the sleep is added to make the text human readable

                    lock.acquire()  #mutex
                    LT_control_information = False
                    lock.release()
                    
            case FollowingTruck1States.spaced_state:
                global spacing_set

                lock.acquire()  #mutex
                spacing_set = True
                lock.release()

                T1 = FollowingTruck1States.synchronise_with_leader_state
                print(T1)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck1States.synchronise_with_leader_state:
                global synchronized

                lock.acquire()  #mutex
                synchronized = True
                lock.release()

                global connection_checker
                connection_checker = 1
                T1 = FollowingTruck1States.confirmed_synchronization_with_leading_truck_state
                print(T1)
                sleep(2) #the sleep is added to make the text human readable

            case  FollowingTruck1States.confirmed_synchronization_with_leading_truck_state:
                global t2_set_spacing

                lock.acquire()  #mutex
                t2_set_spacing = True
                lock.release()

                T1 = FollowingTruck1States.t2_space_confirmation
                print(T1)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck1States.t2_space_confirmation:
                global t2_spacing_set
                if t2_spacing_set:
                    T1 = FollowingTruck1States.synchronize_with_truck2_state
                    print(T1)
                    sleep(2) #the sleep is added to make the text human readable
                    
                    lock.acquire()  #mutex
                    t2_spacing_set = False
                    lock.release()

            case FollowingTruck1States.synchronize_with_truck2_state:
                T1 =FollowingTruck1States.idle_state
                print("Following Truck1 has synchronised with the leading truck and waiting for waiting cordinates")
                print(T1)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck1States.movement_state:
                global LT_control_information1

                lock.acquire()  #mutex
                LT_control_information1 = True
                lock.release()

                T1 = FollowingTruck1States.ready_to_receive_other_control_information_state
                print(T1)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck1States.ready_to_receive_other_control_information_state:
                T1 = FollowingTruck1States.idle_state
                print(T1)
                sleep(2) #the sleep is added to make the text human readable
            
def FOLLOWINGTRUCK2():
    while True:
        global T2
        match T2:
            case FollowingTruck2States.idle_state:
                global t2_set_spacing
                global LT_control_information1
                if t2_set_spacing:
                    T2 = FollowingTruck2States.t2_spaced
                    print(T2)
                    sleep(2) #the sleep is added to make the text human readable

                    lock.acquire()  #mutex
                    t2_set_spacing = False
                    lock.release()
                
                elif LT_control_information1:
                    T2 =  FollowingTruck2States.movement_state
                    print(T2)
                    sleep(2) #the sleep is added to make the text human readable

                    lock.acquire()  #mutex
                    LT_control_information1 = False
                    lock.release()
            
            case FollowingTruck2States.t2_spaced:
                global t2_spacing_set

                lock.acquire()  #mutex
                t2_spacing_set = True
                lock.release()

                T2 = FollowingTruck2States.synchronise_with_truck1_state
                print(T2)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck2States.synchronise_with_truck1_state:
                global truck2_connected #we are updating a global variable so we have to lock it to avoid racing condition

                lock.acquire()  #mutex
                truck2_connected =  True
                lock.release()

                T2 =  FollowingTruck2States.idle_state
                print(T2)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck2States.movement_state:
                global T2_carried_out_control_information

                lock.acquire()
                T2_carried_out_control_information = True
                lock.release()

                T2 = FollowingTruck2States.ready_to_receive_other_control_information_state
                print(T2)
                sleep(2) #the sleep is added to make the text human readable

            case FollowingTruck2States.ready_to_receive_other_control_information_state:
                T2 = FollowingTruck2States.idle_state
                print(T2)
                sleep(2) #the sleep is added to make the text human readable


#############################################################################################################################################################################################





#############################################################################################################################################################################################
#Thread section, to make the trucks move in parallel

#I create an instance of the thread
t1 = Thread(target=LEADINGTRUCK)
t2 = Thread(target=FOLLOWINGTRUCK1)
t3 = Thread(target=FOLLOWINGTRUCK2)

#I start the thread by call the ".start()" method
t1.start()
t2.start()
t3.start()


#And call the ".join()"" function, so that the main thread can wait for the threads to finish before terminating
t1.join()
t2.join()
t3.join()




#Python "match case " has build in break in the case