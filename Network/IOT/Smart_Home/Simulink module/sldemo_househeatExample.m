
mdl='sldemo_househeat';
open_system(mdl);


open_system([mdl,'/Thermostat']);
%%
eheatExample');open_system('sldemo_househeat/Heater')

open_system([mdl,'/Heater']);

open_system([mdl,'/House']);

evalc('sim(mdl)');
open_system([mdl '/PlotResults']),

close_system(mdl);

