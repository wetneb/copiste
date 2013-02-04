Copiste project
==================
Antonin Delpeuch

Based on :
* libvlc : see http://videolan.org/
* Qt : see http://doc.trolltech.com/
* boost : see http://boost.org/
* libchromaprint : see http://acoustid.org/chromaprint

This project provides a small set of tools around audio classification.

`nnat` : Neural Network Analysis Tool
------------------------------------
This tool takes a neural network described in an XML format
(see the networks directory for examples) and trains it to
fit a given corpus, described in another XML format (see the
corpus directory for examples). It can also evaluate how good
a network fits a corpus, and provides a small graphical interface
that can be used to visualize 2D networks.

`featdraw` : Feature Drawer
----------------------------
This tool takes an audio file and draws the variations of some
features in this file. The features are described by a pipeline
(XML file, see the pipeline directory), allowing the user to
change on the fly how features are computed.

`mkcorpus` : Make Corpus
----------------------
This tool uses a pipeline and a set of audio files to create
a corpus (as in nnat) where the features are written.
It uses another XML file, which defines the different audio
classes.

`specdraw` : Draw Spectrum
----------------------------
This tool just takes as input an audio file and draws the
variations of the frequency spectrum during the time.

`classify` : Classify an audio file
---------------------------------
This program takes an audio file, a pipeline and a network, and
says what class the file belongs to.

`live` : Live stream classification with neural networks
-------------------------------------------------------
This program reads a live stream (e.g. a radio), and shows the
variations of the features and the classification of the stream,
using a neural network.

`fingerprint` : Live stream classification with hidden Markov models
---------------------------------------------------------------------
This program does the same thing as `live` but using HMM. It is
currently under development.

See help pages of all these tools for specific usage instructions.
