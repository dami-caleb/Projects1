%% Thermal Model of a House
%
% This example shows how to use Simulink(R) to create the thermal model of
% a house. This system models the outdoor environment, the thermal
% characteristics of the house, and the house heating system.
%        
% The
% <matlab:cd(setupExample('simulink_general/sldemo_househeatExample'));edit('sldemo_househeat_data.m')
% sldemo_househeat_data.m> file initializes data in the model workspace. To
% make changes, you can edit the model workspace directly or edit the file
% and re-load the model workspace. To view the model workspace, from the
% Simulink Editor *Modeling* tab, click *Model Explorer*.
%   Copyright 2004-2012 The MathWorks, Inc.
%% Open Model
% First, open the
% <matlab:openExample('simulink_general/sldemo_househeatExample');
% sldemo_househeat> model.
mdl='sldemo_househeat';
open_system(mdl);
%% Initialize Model
%
% This model calculates heating costs for a generic house. Opening the
% model loads the information about the house from the
% <matlab:cd(setupExample('simulink_general/sldemo_househeatExample'));edit('sldemo_househeat_data.m')
% sldemo_househeat_data.m> file.  The file does the following:
%
% * Defines the house geometry (size, number of windows)
% * Specifies the thermal properties of house materials
% * Calculates the thermal resistance of the house
% * Provides the heater characteristics (temperature of the hot air, flow-rate)
% * Defines the cost of electricity (0.09$/kWhr)
% * Specifies the initial room temperature (20 &ordm;C = 68 &ordm;F)

%%
% * *Note:* Time is given in units of hours. Certain quantities, like air
% flow-rate, are expressed per hour (not per second).

%%  Model Components

%%
% *Set Point*
%
% The Set Point is a constant block. It specifies the temperature that must
% be maintained indoors. By default, it is 70 &ordm;F. Temperatures are
% given in &ordm;F. The model converts the temperature to &ordm;C.

%%
% *Thermostat*
%
% The Thermostat subsystem contains a Relay block. The thermostat allows
% fluctuations of 5 &ordm;F above or below the desired room temperature. If
% air temperature drops below 65 &ordm;F, the thermostat turns on the
% heater.
%
% Open the
% <matlab:openExample('simulink_general/sldemo_househeatExample');open_system('sldemo_househeat/Thermostat')
% Thermostat>  subsystem.

open_system([mdl,'/Thermostat']);
%%
% *Heater*
%
% The Heater subsystem models a constant air flow rate, |Mdot|, which is
% specified in the
% <matlab:cd(setupExample('simulink_general/sldemo_househeatExample'));edit('sldemo_househeat_data.m')
% sldemo_househeat_data.m> file. The thermostat signal turns the heater on
% or off. When the heater is on, it blows hot air at temperature THeater
% (50 &ordm;C = 122 &ordm;F by default) at a constant flow rate of Mdot
% (1kg/sec = 3600kg/hr by default). Equation 1 expresses the heat flow into
% the room.
%%
% *Equation 1*
%
% $$\frac{dQ}{dt}=\left( T_{heater} - T_{room} \right) \cdot Mdot \cdot c$$
%
% $$\frac{dQ}{dt} = \mbox{ heat flow from the heater into the room}$$
%
% $$c = \mbox{ heat capacity of air at constant pressure}$$
%
% $$Mdot = \mbox{ air mass flow rate through heater (kg/hr)}$$
%
% $$T_{heater} = \mbox{ temperature of hot air from heater}$$
%
% $$T_{room} = \mbox{ current room air temperature}$$
%
% Open the
% <matlab:openExample('simulink_general/sldemo_househeatExample');open_system('sldemo_househeat/Heater')
% Heater>  subsystem.

open_system([mdl,'/Heater']);
%%
% *Cost Calculator*
%
% The Cost Calculator is a Gain block. Cost Calculator integrates the heat
% flow over time and multiplies it by the energy cost.  The cost of heating
% The model plots the heating cost in the PlotResults scope.

%%
% *House*
%
% The House is a subsystem that calculates room temperature variations. It takes
% into consideration the heat flow from the heater and heat losses to the
% environment. Heat losses and the temperature time derivative are expressed by
% Equation 2.

%% 
% *Equation 2*
%
% $$\left( \frac{dQ}{dt} \right) _{losses} = \frac{T_{room}-T_{out}}{R_{eq}}$$
%
% $$\frac{dT_{room}}{dt} = \frac{1}{M_{air} \cdot c} \cdot \left(  \frac{dQ_{heater}}{dt} - \frac{dQ_{losses}}{dt} \right) $$
%
% $$M_{air} = \mbox{ mass of air inside the house}$$
%
% $$R_{eq} = \mbox{ equivalent thermal resistance of the house}$$
%
% Open the
% <matlab:openExample('simulink_general/sldemo_househeatExample');open_system('sldemo_househeat/House')
% House>  subsystem.
open_system([mdl,'/House']);
%% 
% *Model the Environment*
%
% To simulate the environment, the model uses a heat sink with infinite
% heat capacity and time varying temperature, Tout.  The constant block Avg
% Outdoor Temp specifies the average air temperature outdoors. The Daily
% Temp Variation Sine Wave block generates daily outdoor temperature
% fluctuations. You can vary these parameters and to see how they affect
% the heating costs.

%% Run Simulation and Visualize Results
%
% Run the simulation. Use the PlotResults scope to visualize the results.
% The scope plots the heat cost and indoor versus outdoor temperatures. The
% temperature outdoor varies sinusoidally. The indoors temperature remains
% within 5 &ordm;C of the Set Point. The Time axis is in hours.
evalc('sim(mdl)');
open_system([mdl '/PlotResults']),
%%
%
% According to this model, it would cost about $30 to heat the house for two
% days. Try varying the parameters and observe the system response.
%%
close_system(mdl);

%% Next Steps
%
% This model calculates the heating costs only. If the temperature of the
% outside air is higher than the room temperature, the room temperature
% will exceed the desired Set Point.
%
% You can modify this model to include an air conditioner. You can implement the
% air conditioner as a modified heater.  To do this, add parameters like the
% following to <matlab:cd(setupExample('simulink_general/sldemo_househeatExample'));edit('sldemo_househeat_data.m') sldemo_househeat_data.m>.
%
% * Cold air output
% * Temperature of the stream from the air conditioner
% * Air conditioner efficiency
%
% To control both the air conditioner and the heater, modify the
% thermostat.