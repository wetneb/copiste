# Just change these three lists :


libpath = [ '/usr/lib',
	    '/usr/local/lib',
	    '/usr/local/share/qt4/lib' ]

cpppath = [ 'include',
	    '/usr/local/share/qt4/include',
	    '/usr/local/share/qt4/include/QtGui',
	    '/usr/local/share/qt4/include/QtCore',
	    '/usr/local/share/qt4/include/QtXml',
	    '/usr/local/share/qt4/include/QtXmlPatterns',
	    '/usr/include/boost' ]

libs = [ 'vlc',
         'vlccore',
	 'QtCore',
	 'QtGui',
	 'QtXml',
	 'QtXmlPatterns',
	 'boost_program_options',
	 'boost_filesystem',
	 'boost_thread',
         'boost_system']

# End of the configuration section

env = Environment(LIBPATH=libpath, CPPPATH = cpppath, LIBS=libs)

neurones_files = [ 'src/algo/corpus.cpp',
		   'src/algo/neuron.cpp',
		   'src/algo/nnetwork.cpp' ]

sound_analyser_files = [ 'src/core/soundanalyser.cpp',
			 'src/core/streamplayer.cpp' ]

features_files = [ 'src/features/hzcrr.cpp',
		   'src/features/lster.cpp',
		   'src/features/zcr.cpp',
		   'src/features/ste.cpp',
		   'src/features/spectrum.cpp' ]

filters_files = [ 'src/filters/filter.cpp',
		  'src/filters/average.cpp',
		  'src/filters/range.cpp',
		  'src/filters/flux.cpp',
		  'src/filters/centroid.cpp',
		  'src/filters/highlowratio.cpp' ]

audio_analysis_files = sound_analyser_files + features_files + filters_files

nnat_files = neurones_files + ['src/gui/editor.cpp',
	     'src/gui/moc_editor.cpp',
	     'src/gui/moc_view2D.cpp',
	     'src/gui/view2D.cpp',
	     'src/main/nnat.cpp' ]

mkcorpus_files = neurones_files + audio_analysis_files + ['src/core/corpusbuilder.cpp',
		 'src/main/mkcorpus.cpp']

classify_files = neurones_files + audio_analysis_files + ['src/core/classifier.cpp',
		 'src/main/classify.cpp']

featdraw_files = neurones_files + audio_analysis_files + ['src/main/featdraw.cpp',
							'src/core/featuredrawer.cpp' ]

specdraw_files = audio_analysis_files + [ 'src/core/spectrumRecorder.cpp',
		  'src/main/specdraw.cpp']

live_files = neurones_files + audio_analysis_files + ['src/main/live.cpp',
		'src/core/liveplayer.cpp', 'src/core/featuredrawer.cpp' ]


env.Program('nnat', nnat_files)
env.Program('mkcorpus', mkcorpus_files)
env.Program('featdraw', featdraw_files)
env.Program('specdraw', specdraw_files)
env.Program('classify', classify_files)
env.Program('live', live_files)
