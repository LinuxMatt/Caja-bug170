#include "config.h"
#include <eggsmclient.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <unistd.h>

void log_gen(const char *func, const char *fmt, ...)
{
	FILE *fd = stderr;
	struct timeb s_timeb;
	ftime(&s_timeb);
	struct tm *d = localtime(&s_timeb.time);
	fprintf(fd, "%02d:%02d:%02d.%03u ", d->tm_hour, d->tm_min, d->tm_sec, s_timeb.millitm);
	fprintf(fd,"[%s pid=%d func=%s] ", PACKAGE, getpid(), func);
	va_list args;
	va_start(args,fmt);
	vfprintf(fd, fmt, args);
	va_end(args);
	fprintf(fd,"\n");
}

static void changed_callback(gpointer user_data)
{
	static int i = 0;
	i++;
	log_gen("changed_callback", "=== CAJA BUG #170. xsmp + dconf + GSettings bug? count=%d", i);
}

int main(int argc, char *argv[])
{
	gboolean version = FALSE;
	gchar **remaining = NULL;
	const GOptionEntry options[] = {
		{ "version", '\0', 0, G_OPTION_ARG_NONE, &version, "Show the version of the program.", NULL},
		{G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &remaining, NULL, "[URI...]"},
		{NULL}
	};
	GOptionContext *context = g_option_context_new("Program");
	g_option_context_add_main_entries(context, options, NULL);
	g_option_context_add_group(context, gtk_get_option_group(TRUE));
	g_option_context_add_group(context, egg_sm_client_get_option_group());
	GError *error = NULL;
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_printerr("Could not parse arguments: %s\n", error->message);
		g_error_free(error);
		return 1;
	}
	g_option_context_free(context);

	GSettings *caja_preferences;
	GSettings *caja_media_preferences;
	GSettings *caja_window_state;
	GSettings *caja_icon_view_preferences;
	GSettings *caja_desktop_preferences;
	GSettings *caja_tree_sidebar_preferences;
	GSettings *caja_compact_view_preferences;
	GSettings *caja_list_view_preferences;
	GSettings *mate_background_preferences;
	GSettings *mate_lockdown_preferences;

	caja_preferences = g_settings_new("org.mate.caja.preferences");
	caja_media_preferences = g_settings_new("org.mate.media-handling");
	caja_window_state = g_settings_new("org.mate.caja.window-state");
	caja_icon_view_preferences = g_settings_new("org.mate.caja.icon-view");
	caja_compact_view_preferences = g_settings_new("org.mate.caja.compact-view");
	caja_desktop_preferences = g_settings_new("org.mate.caja.desktop");
	caja_tree_sidebar_preferences = g_settings_new("org.mate.caja.sidebar-panels.tree");
	caja_list_view_preferences = g_settings_new("org.mate.caja.list-view");
	mate_background_preferences = g_settings_new("org.mate.background");
	mate_lockdown_preferences = g_settings_new("org.mate.lockdown");

#define CAJA_PREFERENCES_DESKTOP_IS_HOME_DIR "desktop-is-home-dir"

	g_signal_connect_swapped(caja_preferences, "changed::" CAJA_PREFERENCES_DESKTOP_IS_HOME_DIR,
					G_CALLBACK(changed_callback), G_OBJECT(caja_preferences));
	gtk_main();
	return EXIT_SUCCESS;
}
