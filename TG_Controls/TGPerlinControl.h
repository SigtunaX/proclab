#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGPerlinControl : NSObject {
	IBOutlet NSColorWell *TGPColorC;
	IBOutlet NSSlider *TGPZoom;
	IBOutlet NSSlider *TGPPersistence;
	IBOutlet NSSlider *TGPOctaves;
	IBOutlet NSPanel *TGPerlinPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_PERLIN t_data; // Texture data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (void) getValues;

@end
