# Copyright (C) 2017  Intel Corporation. All rights reserved.
# Your use of Intel Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Intel Program License 
# Subscription Agreement, the Intel Quartus Prime License Agreement,
# the Intel FPGA IP License Agreement, or other applicable license
# agreement, including, without limitation, that your use is for
# the sole purpose of programming logic devices manufactured by
# Intel and sold by Intel or its authorized distributors.  Please
# refer to the applicable agreement for further details.

# Quartus Prime: Generate Tcl File for Project
# File: X4_mimo_top.tcl
# Generated on: Thu Feb 24 16:11:01 2022

# Load Quartus Prime Tcl Project package
package require ::quartus::project

set need_to_close_project 0
set make_assignments 1

# Check that the right project is open
if {[is_project_open]} {
	if {[string compare $quartus(project) "X4_mimo_top"]} {
		puts "Project X4_mimo_top is not open"
		set make_assignments 0
	}
} else {
	# Only open if not already open
	if {[project_exists X4_mimo_top]} {
		project_open -revision X4_mimo_top X4_mimo_top
	} else {
		project_new -revision X4_mimo_top X4_mimo_top
	}
	set need_to_close_project 1
}

# Make assignments
if {$make_assignments} {
	set_global_assignment -name FAMILY "Cyclone IV E"
	set_global_assignment -name DEVICE EP4CE15F23C8
	set_global_assignment -name TOP_LEVEL_ENTITY x4_mimo_top
	set_global_assignment -name ORIGINAL_QUARTUS_VERSION 17.1.0
	set_global_assignment -name PROJECT_CREATION_TIME_DATE "14:31:07  FEBRUARY 24, 2022"
	set_global_assignment -name LAST_QUARTUS_VERSION "17.1.0 Standard Edition"
	set_global_assignment -name VERILOG_FILE ../src/data_process/frame_data_fifo.v
	set_global_assignment -name VERILOG_FILE ../src/data_process/data_process.v
	set_global_assignment -name VERILOG_FILE ../src/debounce/udp_debounce.v
	set_global_assignment -name VERILOG_FILE ../src/debounce/arp_debounce.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/generate_data.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_udp_send.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_top.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_receive.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_mac_send.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_data_wr2ram.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/eth_arp_send.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/crc.v
	set_global_assignment -name VERILOG_FILE ../src/ethernet/auto_read_write.v
	set_global_assignment -name QSYS_FILE ../src/ip_core/global_clk.qsys
	set_global_assignment -name VERILOG_FILE ../src/spi_driver/spi_module.v
	set_global_assignment -name VERILOG_FILE ../src/switch_driver/switch.v
	set_global_assignment -name VERILOG_FILE ../src/x4driver/X4_read_frame.v
	set_global_assignment -name VERILOG_FILE ../src/x4driver/ram.v
	set_global_assignment -name QIP_FILE ../src/x4driver/ram.qip
	set_global_assignment -name VERILOG_FILE ../src/x4driver/cmd_ctrl.v
	set_global_assignment -name VERILOG_FILE ../src/x4_mimo_top.v
	set_global_assignment -name VERILOG_FILE ../src/syn_rst_n.v
	set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
	set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85
	set_global_assignment -name POWER_PRESET_COOLING_SOLUTION "23 MM HEAT SINK WITH 200 LFPM AIRFLOW"
	set_global_assignment -name POWER_BOARD_THERMAL_MODEL "NONE (CONSERVATIVE)"
	set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top
	set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top
	set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top
	set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
	set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files
	set_location_assignment PIN_A12 -to gmii_rx_clk
	set_location_assignment PIN_C1 -to gmii_rx_data[7]
	set_location_assignment PIN_C7 -to gmii_rx_data[6]
	set_location_assignment PIN_C8 -to gmii_rx_data[5]
	set_location_assignment PIN_C2 -to gmii_rx_data[4]
	set_location_assignment PIN_D2 -to gmii_rx_data[3]
	set_location_assignment PIN_E1 -to gmii_rx_data[2]
	set_location_assignment PIN_F1 -to gmii_rx_data[1]
	set_location_assignment PIN_F2 -to gmii_rx_data[0]
	set_location_assignment PIN_C6 -to gmii_rx_dv
	set_location_assignment PIN_C3 -to gmii_rx_er
	set_location_assignment PIN_B2 -to gmii_tx_clk
	set_location_assignment PIN_A8 -to gmii_tx_data[7]
	set_location_assignment PIN_B7 -to gmii_tx_data[6]
	set_location_assignment PIN_A7 -to gmii_tx_data[5]
	set_location_assignment PIN_B6 -to gmii_tx_data[4]
	set_location_assignment PIN_B5 -to gmii_tx_data[3]
	set_location_assignment PIN_A5 -to gmii_tx_data[2]
	set_location_assignment PIN_B4 -to gmii_tx_data[1]
	set_location_assignment PIN_B3 -to gmii_tx_data[0]
	set_location_assignment PIN_A3 -to gmii_tx_en
	set_location_assignment PIN_B8 -to gmii_tx_er
	set_location_assignment PIN_A4 -to phy_rst_n
	set_location_assignment PIN_U13 -to s2_in
	set_location_assignment PIN_V13 -to s3_in
	set_location_assignment PIN_AB12 -to s_rst_n
	set_location_assignment PIN_F13 -to x4_cs
	set_location_assignment PIN_E14 -to x4_enable
	set_location_assignment PIN_F14 -to x4_isr
	set_location_assignment PIN_K16 -to x4_miso
	set_location_assignment PIN_J17 -to x4_mosi
	set_location_assignment PIN_H17 -to x4_sclk
	set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top

	# Including default assignments
	set_global_assignment -name TIMEQUEST_REPORT_WORST_CASE_TIMING_PATHS ON -family "Cyclone IV E"
	set_global_assignment -name TIMEQUEST_CCPP_TRADEOFF_TOLERANCE 0 -family "Cyclone IV E"
	set_global_assignment -name TDC_CCPP_TRADEOFF_TOLERANCE 0 -family "Cyclone IV E"
	set_global_assignment -name TIMEQUEST_DO_CCPP_REMOVAL ON -family "Cyclone IV E"
	set_global_assignment -name TIMEQUEST_SPECTRA_Q OFF -family "Cyclone IV E"
	set_global_assignment -name SYNTH_TIMING_DRIVEN_SYNTHESIS ON -family "Cyclone IV E"
	set_global_assignment -name SYNCHRONIZATION_REGISTER_CHAIN_LENGTH 2 -family "Cyclone IV E"
	set_global_assignment -name SYNTH_RESOURCE_AWARE_INFERENCE_FOR_BLOCK_RAM ON -family "Cyclone IV E"
	set_global_assignment -name OPTIMIZE_HOLD_TIMING "ALL PATHS" -family "Cyclone IV E"
	set_global_assignment -name OPTIMIZE_MULTI_CORNER_TIMING ON -family "Cyclone IV E"
	set_global_assignment -name AUTO_DELAY_CHAINS ON -family "Cyclone IV E"
	set_global_assignment -name CRC_ERROR_OPEN_DRAIN OFF -family "Cyclone IV E"
	set_global_assignment -name USE_CONFIGURATION_DEVICE OFF -family "Cyclone IV E"
	set_global_assignment -name ENABLE_OCT_DONE OFF -family "Cyclone IV E"

	# Commit assignments
	export_assignments

	# Close project
	if {$need_to_close_project} {
		project_close
	}
}
