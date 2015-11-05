dofile(os.getenv("OPENVIBE_HOME") .. "/share/openvibe-plugins/stimulation/lua-stimulator-stim-codes.lua")

stim = OVTK_StimulationId_Label_00
-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");

	-- inspects the box topology
--	io.write(string.format("box has %i input(s)\n", box:get_input_count()))
--	io.write(string.format("box has %i output(s)\n", box:get_output_count()))
--	io.write(string.format("box has %i setting(s)\n", box:get_setting_count()))
--	for i = 1, box:get_setting_count() do
--		io.write(string.format(" - setting %i has value [%s]\n", i, box:get_setting(i)))
--	end

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

		if box:get_stimulation_count(1)==1 then -- stimulation received
			box:send_stimulation(1,stim,t,0)
			if stim == OVTK_StimulationId_Label_00 then
				stim = OVTK_StimulationId_Label_01
			else
				stim = OVTK_StimulationId_Label_00
			end
			box:remove_stimulation(1, 1)
		end
		-- releases cpu
		box:sleep()
	end
end
