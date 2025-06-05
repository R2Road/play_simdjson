#if defined( DEBUG ) || defined( _DEBUG )
	#include "vld/include/vld.h"
#endif

#include "r2tm/r2tm_Director.h"
#include "r2tm/r2tm_WindowsUtility.h"

#include "menu_root.h"

int main()
{
	//
	// Environment : Title
	//
	r2tm::WindowsUtility::ChangeTitle( "play_simdjson" );

	//
	// Environment : Size
	//
	r2tm::WindowsUtility::Resize( 960, 960 );

	//
	// Environment : Position
	//
	r2tm::WindowsUtility::Move( 0, 0 );

	//
	// Environment : Quick Edit Off
	//
	r2tm::WindowsUtility::QuickEditEnable( false );

	//
	// Setup
	//
	r2tm::Director director;
	director.Setup( Menu_Root() );

	//
	// Process
	//
	director.Run();

	return 0;
}
