#import <Cocoa/Cocoa.h>

@interface tableviewLYR : NSObject {
	NSMutableArray				*records;
	IBOutlet NSTableView		*TableView;
}
-(void)addLayer:(NSString*)enabled image:(NSImage*)thumb operation:(int)oper_ind properties:(NSString*)props;
-(void)deleteLayerAtPos: (int)pos;
-(void)deleteallLayers;

@end
