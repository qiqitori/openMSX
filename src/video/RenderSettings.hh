// $Id$

#ifndef __RENDERSETTINGS_HH__
#define __RENDERSETTINGS_HH__

#include "IntegerSetting.hh"
#include "FloatSetting.hh"
#include "BooleanSetting.hh"
#include "RendererFactory.hh"
#include "FrameSkipSetting.hh"
#include "Scaler.hh"
#include "InfoTopic.hh"


namespace openmsx {

class MSXConfig;
class InfoCommand;


/** Singleton containing all settings for renderers.
  * Keeping the settings here makes sure they are preserved when the user
  * switches to another renderer.
  */
class RenderSettings
{
public:
	/** Render accuracy: granularity of the rendered area.
	  */
	enum Accuracy { ACC_SCREEN, ACC_LINE, ACC_PIXEL };

	static RenderSettings& instance();

	/** Accuracy [screen, line, pixel] */
	EnumSetting<Accuracy>* getAccuracy() { return accuracy; }

	/** Deinterlacing [on, off]. */
	BooleanSetting* getDeinterlace() { return deinterlace; }

	/** The current frameskip. */
	FrameSkipSetting* getFrameSkip() { return frameSkip; }

	/** Full screen [on, off]. */
	BooleanSetting* getFullScreen() { return fullScreen; }

	/** The amount of gamma correction. */
	FloatSetting* getGamma() { return gamma; }

	/** The amount of glow [0..100]. */
	IntegerSetting* getGlow() { return glow; }

	/** The amount of horizontal blur [0..100]. */
	IntegerSetting* getHorizontalBlur() { return horizontalBlur; }

	/** The current renderer. */
	RendererFactory::RendererSetting* getRenderer() { return renderer; }

	/** The current scaling algorithm. */
	EnumSetting<ScalerID>* getScaler() { return scaler; }

	/** The alpha value [0..100] of the scanlines. */
	IntegerSetting* getScanlineAlpha() { return scanlineAlpha; }

private:
	RenderSettings();
	~RenderSettings();

	// Please keep the settings ordered alphabetically.
	EnumSetting<Accuracy>* accuracy;
	BooleanSetting* deinterlace;
	FrameSkipSetting* frameSkip;
	BooleanSetting* fullScreen;
	FloatSetting* gamma;
	IntegerSetting* glow;
	IntegerSetting* horizontalBlur;
	RendererFactory::RendererSetting* renderer;
	EnumSetting<ScalerID>* scaler;
	IntegerSetting* scanlineAlpha;

	class RendererInfo : public InfoTopic {
	public:
		RendererInfo(RenderSettings& parent);
		virtual string execute(const vector<string> &tokens) const
			throw();
		virtual string help   (const vector<string> &tokens) const
			throw();
	private:
		RenderSettings& parent;
	} rendererInfo;

	MSXConfig& msxConfig;
	InfoCommand& infoCommand;
};

} // namespace openmsx

#endif // __RENDERSETTINGS_HH__

