# $Id$

noinst_LIBRARIES = libui.a

libui_a_SOURCES = colors.h secstring.h basewindow.h curswa.h inputwidget.h	\
uiexception.h passwordwidget.h dialogbox.h basewindow.cc inputwidget.cc		\
messagebox.h messagebox.cc listwidget.h button.h button.cc passwordwidget.cc	\
dialogbox.cc colors.cc misc.h misc.cc

libui_a_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"

if USE_INCLUDED_LIBINTL
libui_a_CPPFLAGS += -I$(top_srcdir)/intl
endif

if DISABLE_ASSERT
libui_a_CPPFLAGS += -DNDEBUG
endif

clean-local:
	rm -rf SunWS_cache
