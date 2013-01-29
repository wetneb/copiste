# Just change these three lists :

import os

libpath = [
    '/usr/lib',
    '/usr/local/lib',
    '/usr/local/share/qt4/lib',
    ]

cpppath = [
    'include',
    '/usr/include/qt4',
    '/usr/include/qt4/QtGui',
    '/usr/include/qt4/QtCore',
    '/usr/include/qt4/QtXml',
    '/usr/include/qt4/QtXmlPatterns',
    '/usr/include/boost',
    ]

libs = [
    'vlc',
    'vlccore',
    'QtCore',
    'QtGui',
    'QtXml',
    'QtXmlPatterns',
    'boost_program_options',
    'boost_filesystem',
    'boost_system',
    'boost_thread',
    'boost_serialization',
    'chromaprint',
    ]

flags = ['-Wall']

# End of the configuration section

env = Environment(LIBPATH=libpath, CPPPATH = cpppath, LIBS=libs, CPPFLAGS = flags,
	          TOOLS = ['default', 'qt'],
		  TOOLPATH = ['.'],
                  ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : os.environ['TERM'],
			 'QTDIR' : '/usr',
   })
neurones_files = [
    'src/algo/corpus.cpp',
    'src/algo/neuralnetwork.cpp',
    ]

sound_analyser_files = [
    'src/core/soundanalyser.cpp',
    'src/core/streamplayer.cpp',
    ]

features_files = [
    'src/features/hzcrr.cpp',
    'src/features/lster.cpp',
    'src/features/zcr.cpp',
    'src/features/ste.cpp',
    'src/features/spectrum.cpp',
    ]

filters_files = [
    'src/filters/filter.cpp',
    'src/filters/average.cpp',
    'src/filters/range.cpp',
    'src/filters/flux.cpp',
    'src/filters/centroid.cpp',
    'src/filters/highlowratio.cpp',
    'src/filters/memory.cpp',
    'src/filters/mel.cpp',
    'src/filters/distance.cpp',
    ]

fp_files = [
    'src/core/fingerprinter.cpp',
    'src/algo/hmm.cpp',
    ]


audio_analysis_files = sound_analyser_files + features_files + filters_files

nnat_files = neurones_files + [
    'src/gui/editor.cpp',
    'include/gui/moc_editor.cc',
    'include/gui/moc_view2D.cc',
    'src/gui/view2D.cpp',
    'src/gui/quadtree.cpp',
    'src/main/nnat.cpp',
    ]

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

fingerprint_files = audio_analysis_files + fp_files + ['src/main/fingerprint.cpp']

env.Moc('include/gui/view2D.h')
env.Moc('include/gui/editor.h')


nnatProg = env.Program('nnat', nnat_files)
mkcorpusProg = env.Program('mkcorpus', mkcorpus_files)
featdrawProg = env.Program('featdraw', featdraw_files)
specdrawProg = env.Program('specdraw', specdraw_files)
classifyProg = env.Program('classify', classify_files)
liveProg = env.Program('live', live_files)
fingerprintProg = env.Program('fingerprint', fingerprint_files)

Default(nnatProg)
Default(mkcorpusProg)
Default(featdrawProg)
Default(specdrawProg)
Default(classifyProg)
Default(liveProg)
Default(fingerprintProg)

otherInstallFiles = Dir('pipeline')

env.Install('/tmp', [nnatProg, mkcorpusProg, otherInstallFiles])
env.Alias('install', '/tmp')
# TBC with http://scons.org/doc/HTML/scons-user/
# the programs have to be designed to be installed (relative paths, aso)
