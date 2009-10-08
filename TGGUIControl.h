//
//  TGGUIControl.h
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h
#import "TG_Controls/TGPlainControl.h"
#import "TG_Controls/TGNoiseControl.h"
#import "TG_Controls/TGCelularControl.h"
#import "TG_Controls/TGPlasmaControl.h"
#import "c_structs.h"
#import "textgenlib/main.h"
#import "controls/tableview.h"

#define TMP_TEXTURES 1

@interface TGGUIControl : NSObject {
	/* Controles de la ventana principal */
	IBOutlet NSView *mainTGView;
	IBOutlet NSImageView *IVFinaltexture;
	IBOutlet NSImageView *IVTemptexture;
	IBOutlet NSComboBox *CBOperation;
	IBOutlet tableviewLYR*	layerlist;	/* Layer array list */
	
	/* Controladores de los paneles "hijos"*/
	IBOutlet TGPlainControl *TGPlainCtrl;
	IBOutlet TGNoiseControl *TGNoiseCtrl;
	IBOutlet TGCelularControl *TGCelularCtrl;
	IBOutlet TGPlasmaControl *TGPlasmaCtrl;

	/* Common-C structures */
	CTextGen	*tg_final;
	CTextGen	*tg_temptext[TMP_TEXTURES];
//	c_gfx	gfx_Finaltext;					// Final texture
//	c_gfx	gfx_Temptext[TMP_TEXTURES];		// Temporary texture
	
}
- (void) hideAllPanels;

- (IBAction) showPlain:(id)sender;
- (void) GetPlainData:(T_PLAIN)t_data;

- (IBAction) showNoise:(id)sender;
- (void) GetNoiseData:(T_NOISE)t_data;

- (IBAction) showCelular:(id)sender;
- (void)GetCelularData:(T_CELULAR)t_data;

- (IBAction) showPlasma:(id)sender;
- (void)GetPlasmaData:(T_PLASMA)t_data;

- (void) SaveToTGA:(id)sender;
- (IBAction) openZNTfile:(id)sender;

- (void) resetTemp:(id)sender;
- (void) renderFinal:(id)sender;
- (void) renderTemp:(id)sender;

- (void) DeleteAllLayers:(id)sender;
- (void) AddLayer:(id)sender;

- (void) AddEffect:(int)type;
- (void) AddEffect_bw:(id)sender;
- (void) AddEffect_r2polar:(id)sender;
- (void) AddEffect_blur:(id)sender;
- (void) AddEffect_mblur:(id)sender;
- (void) AddEffect_edges1:(id)sender;
- (void) AddEffect_edges2:(id)sender;
- (void) AddEffect_sharpen1:(id)sender;
- (void) AddEffect_sharpen2:(id)sender;
- (void) AddEffect_sharpen3:(id)sender;
- (void) AddEffect_emboss1:(id)sender;
- (void) AddEffect_emboss2:(id)sender;
- (void) AddEffect_mean1:(id)sender;
- (void) AddEffect_mean2:(id)sender;
- (void) AddEffect_custom3x3:(id)sender;

- (void) DeleteLayer:(id)sender;
- (void) UpdateLayerList:(id)sender;
- (void) LayerUp:(id)sender;
- (void) LayerDown:(id)sender;
- (void) UpdateOperationFromLayer:(int)layer_num AndOperation:(int)operation;

- (void) LogFinalTexInfo;
- (void) LogTmpTexInfo;


#pragma mark Utilities

- (void) init_prepareText;

#pragma mark Accessors

// Llista de layers
//- (NSMutableArray*)layerList;
//- (void)setLayerList:(NSArray*)aValue;

@end


/////////////////////////////////////////////////////////////////
/// NSImage EXTRAS
/////////////////////////////////////////////////////////////////

@interface NSImage (Extras)
- (NSImage*)imageByScalingProportionallyToSize:(NSSize)aSize;
@end
