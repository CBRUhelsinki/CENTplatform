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
	io.write("uninitialize has been called\n")
end

-- this function is called once by the box
function process(box)
	io.write("process has been called\n")

	-- enters infinite loop
	-- cpu will be released with a call to sleep
	-- at the end of the loop
	while true do

		-- gets current simulated time
		t = box:get_current_time()

		if box:get_stimulation_count(1)==1 and box:get_stimulation_count(2)==1  then --both stimulations received
			
			id1,date1,dur1 = box:get_stimulation(1,1)
			id2,date2,dur2 = box:get_stimulation(2,1)
			
			if id1 == OVTK_StimulationId_Target and id2 == OVTK_StimulationId_Target then
				box:send_stimulation(1,OVTK_GDF_Correct,t,0)
			else
				box:send_stimulation(1,OVTK_GDF_Incorrect,t,0)
			end
			box:remove_stimulation(1, 1)
			box:remove_stimulation(2, 1)
		end
		-- releases cpu
		box:sleep()
	end
end
