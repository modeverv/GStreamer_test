// ConsoleApplication2.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <gst/gst.h>

int main(int argc, char *argv[])
{
	printf("aaaaa\n");

	GString *launch_str;
	GstElement *pipeline;
	GstBus *bus;
	GstMessage *msg;

	gst_init(&argc, &argv);

	launch_str = g_string_new(NULL);
	//	g_string_printf(launch_str,
	//		"playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
	g_string_printf(launch_str,
		"dx9screencapsrc num-buffers=1 ! jpegenc ! filesink location=C:/tmp/d.jpg", NULL);

	pipeline = gst_parse_launch(launch_str->str, NULL);

	if (pipeline == NULL) {
		g_print("parse_launch fail.\n");
		return 1;
	}

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	bus = gst_element_get_bus(pipeline);
	gint handle_msg;
	handle_msg = GST_MESSAGE_EOS | GST_MESSAGE_ERROR;
	msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)handle_msg);

	GError *err;
	gchar *debug_info;
	if (msg != NULL)
		switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_ERROR:
			gst_message_parse_error(msg, &err, &debug_info);
			g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
			g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
			g_clear_error(&err);
			g_free(debug_info);
			break;
		case GST_MESSAGE_EOS:
			g_print("End-Of-Stream reached.\n");
			break;
		default:
			/* We should not reach here because we only asked for ERRORs and EOS */
			g_printerr("Unexpected message received.\n");
			break;
		}
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
