dofile(os.getenv("OPENVIBE_HOME") .. "/share/openvibe-plugins/stimulation/lua-stimulator-stim-codes.lua")

-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");

	-- inspects the box topology
	io.write(string.format("box has %i input(s)\n", box:get_input_count()))
	io.write(string.format("box has %i output(s)\n", box:get_output_count()))
	io.write(string.format("box has %i setting(s)\n", box:get_setting_count()))
	for i = 1, box:get_setting_count() do
		io.write(string.format(" - setting %i has value [%s]\n", i, box:get_setting(i)))
	end

	

end

-- this function is called when the box is uninitialized
function uninitialize(box)
	
end

function writeConfig(inputfile,outputfile)
	io.input(inputfile)
	io.output(outputfile)
	
	
	val = io.read("*line") -- header line
	val = io.read("*line") -- data line
	t_val = string.sub(val,10,17) -- Extract baseline value
	io.write(string.format("<OpenViBE-SettingsOverride>\n\t<SettingValue>%0.2f</SettingValue>\n</OpenViBE-SettingsOverride>",t_val))
	
end	
	
-- this function is called once by the box
function process(box)
	filewrite = false
	-- io.write("process has been called\n")
	-- dofile("../share/openvibe-plugins/stimulation/writeit.lua")
	-- enters infinite loop
	-- cpu will be released with a call to sleep
	-- at the end of the loop
	while true do
	
		t = box:get_current_time()
		
		if not filewrite then
		
			writeConfig( os.getenv("CENT_CURRENT_SESSION") .. "/betaval.csv",  os.getenv("CENT_CURRENT_SESSION") .. "/betabase.cfg")					
			writeConfig( os.getenv("CENT_CURRENT_SESSION") .. "/thetaval.csv", os.getenv("CENT_CURRENT_SESSION") .. "/thetabase.cfg")
				
			filewrite=true
			box:send_stimulation(1, OVTK_StimulationId_ExperimentStop, t+1, 0)
		end
		box:sleep()
	end
end