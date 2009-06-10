//
//  GLImage.m
//  GeneratorLab

#import "GLImage.h"


@implementation GLImage

- (id)init
{
    if (self = [super init])
    {
        [self setTitle:@"Default Layer"];
        [self setVisible:nil];
		[self setOperation:nil];
        [self setDefaultThumbnail:nil];
    }
    return self;
}

- (void)dealloc
{
    [self setTitle:nil];
    [self setVisible:nil];
	[self setOperation:nil];
    [self setDefaultThumbnail:nil];
    
    [super dealloc];
}


#pragma mark -
#pragma mark Accessors

- (NSButton*)visible
{
    return _visible;
}

- (void)setVisible:(NSButton*)aValue
{
    NSButton* oldVisible = _visible;
    _visible = [aValue retain];
    [oldVisible release];
}

- (NSImage*)defaultThumbnail
{
    return _defaultThumbnail;
}

- (void)setDefaultThumbnail:(NSImage*)aValue
{
    NSImage* oldDefaultThumbnail = _defaultThumbnail;
    _defaultThumbnail = [aValue retain];
    [oldDefaultThumbnail release];
}

- (NSComboBoxCell*)operation
{
    return _operation;
}

- (void)setOperation:(NSComboBoxCell*)aValue
{
    NSComboBoxCell* oldOperation = _operation;
    _operation = [aValue retain];
    [oldOperation release];
}

- (NSString*)title
{
    return _title;
}

- (void)setTitle:(NSString*)aValue
{
    NSString* oldTitle = _title;
    _title = [aValue copy];
    [oldTitle release];
}

@end
