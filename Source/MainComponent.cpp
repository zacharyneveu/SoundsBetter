#include "MainComponent.hpp"

using namespace juce;
using namespace dsp;

//==============================================================================
MainComponent::MainComponent() : dev_selector (deviceManager, 1, 2, 2, 2, false, false, false, true)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    SBLookAndFeel* lf = new SBLookAndFeel;
    setLookAndFeel (lf);

    addAndMakeVisible (hpcf);

    addAndMakeVisible (dev_selector);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::writeExternalStorage)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::writeExternalStorage))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::writeExternalStorage,
                                           [&] (bool granted) {});
    }

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (
            juce::RuntimePermissions::recordAudio,
            [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    File f = hpcf.getHPCF();
    if (f.existsAsFile())
    {
        try
        {
            int size = 0;
            conv.loadImpulseResponse (hpcf.getHPCF(),
                                      Convolution::Stereo::no,
                                      Convolution::Trim::no,
                                      size,
                                      Convolution::Normalise::yes);
            ProcessSpec spec;
            spec.maximumBlockSize = samplesPerBlockExpected;
            spec.numChannels = 2;
            spec.sampleRate = sampleRate;

            conv.prepare (spec);
        }
        catch (...)
        {
            DBG ("Problems loading " + hpcf.getHPCF().getFullPathName());
        }
    }
}

    void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
    {
        AudioBlock<float> block (*bufferToFill.buffer);
        ProcessContextReplacing<float> ctxt (block);
        conv.process (ctxt);
    }

    void MainComponent::releaseResources()
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void MainComponent::paint (juce::Graphics & g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void MainComponent::resized()
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        Rectangle<int> bounds = getBounds();
        hpcf.setBounds (bounds.removeFromTop (200));

        dev_selector.setBounds (bounds.removeFromTop (200));
    }
