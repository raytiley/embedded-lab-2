#include "menu.h"
#include <stdio.h>
#include <inttypes.h>
#include "pd_controller.h"
#include "interpolator.h"
#include "logging.h"

// GLOBALS
extern char logging;

extern long g_target_position;
extern long g_current_position;
extern int g_velocity;
extern long g_velocity_last_position;
extern long g_motor_output;
extern long g_prop_gain;
extern long g_der_gain;
extern int g_velocity;


// local "global" data structures
char receive_buffer[32];
unsigned char receive_buffer_position;
char send_buffer[32];

char command_buffer[32];
unsigned char command_position;

// A generic function for whenever you want to print to your serial comm window.
// Provide a string and the length of that string. My serial comm likes "\r\n" at 
// the end of each string (be sure to include in length) for proper linefeed.
void print_usb( char *buffer, int n ) {
	serial_send( USB_COMM, buffer, n );
	wait_for_sending_to_finish();
}	
		
//------------------------------------------------------------------------------------------
// Initialize serial communication through USB and print menu options
// This immediately readies the board for serial comm
void init_menu() {
	// Set the baud rate to 9600 bits per second.  Each byte takes ten bit
	// times, so you can get at most 960 bytes per second at this speed.
	serial_set_baud_rate(USB_COMM, 9600);

	// Start receiving bytes in the ring buffer.
	serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));

	//memcpy_P( send_buffer, PSTR("USB Serial Initialized\r\n"), 24 );
	//snprintf( printBuffer, 24, "USB Serial Initialized\r\n");
	//print_usb( printBuffer, 24 );
	print_usb( "USB Serial Initialized\r\n", 24);
}

//------------------------------------------------------------------------------------------
// process_received_byte: Parses a menu command (series of keystrokes) that 
// has been received on USB_COMM and processes it accordingly.
// The menu command is buffered in check_for_new_bytes_received (which calls this function).
void process_received_string(const char* buffer)
{	
	// parse and echo back to serial comm window (and optionally the LCD)
	char op_char;
	long value;
	sscanf(buffer, "%c %ld", &op_char, &value);

	// Check valid command and implement
	switch (op_char) {
		// change toggle frequency for <color> LED
		case 'R':
		case 'r':
			set_target((long)(((float)value / 360.0) * 64));
			break;
		case 'L':
		case 'l':
			if(logging) {
				logging = 0;
				print_usb("\r\n\r\n", 4);
			}
			else {
				log_print_header();
				logging = 1;
			}
			break;
		case 'V':
		case 'v':
			log_print_log_row();
			break;
		case 'P':
			increment_proportional_gain(value);
			break;
		case 'p':
			decrement_proportional_gain(value);
			break;
		case 'D':
			increment_derivative_gain(value);
			break;
		case 'd':
			decrement_derivative_gain(value);
			break;  
		default:
			print_usb( "Command does not compute.\r\n", 27 );
		} // end switch(op_char) 
} //end menu()

//---------------------------------------------------------------------------------------
// If there are received bytes to process, this function loops through the receive_buffer
// accumulating new bytes (keystrokes) in another buffer for processing.
void check_for_new_bytes_received()
{
	
	char menuBuffer[32];
	static int received = 0;
	
	// while there are unprocessed keystrokes in the receive_buffer, grab them and buffer
	// them into the menuBuffer
	while(serial_get_received_bytes(USB_COMM) != receive_buffer_position)
	{
		// place in a buffer for processing
		menuBuffer[received] = receive_buffer[receive_buffer_position];
		++received;
		
		// Increment receive_buffer_position, but wrap around when it gets to
		// the end of the buffer. 
		if ( receive_buffer_position == sizeof(receive_buffer) - 1 )
		{
			receive_buffer_position = 0;
		}			
		else
		{
			receive_buffer_position++;
		}
	}
	// If there were keystrokes processed, check if a menue command
	if (received) {
		for (int i=0; i<received; i++)
		{
			//If first character is $ or ! (start / execute) act accordingly
			switch(menuBuffer[i]) {
				case '$':
					//Reset
					command_position = 0;
					break;
				case '!':
					//Execute command
					command_buffer[command_position] = '\0';
					process_received_string(command_buffer);
					command_position = 0;
					break;
				default:
					command_buffer[command_position] = menuBuffer[i];
					command_position++;
					break;
			}

		}
		received = 0;
	}
}
	
//-------------------------------------------------------------------------------------------
// wait_for_sending_to_finish:  Waits for the bytes in the send buffer to
// finish transmitting on USB_COMM.  We must call this before modifying
// send_buffer or trying to send more bytes, because otherwise we could
// corrupt an existing transmission.
void wait_for_sending_to_finish()
{
	while(!serial_send_buffer_empty(USB_COMM))
		serial_check();		// USB_COMM port is always in SERIAL_CHECK mode
}