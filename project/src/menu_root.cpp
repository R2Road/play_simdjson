#include "menu_root.h"

#include <string>

#include "r2tm/r2tm_MenuProcessor.h"
#include "r2tm/r2tm_VersionInfo.h"

#include "test/test_simdjson.h"

r2tm::TitleFunctionT Menu_Root::GetTitleFunction() const
{
	return []()->const char*
	{
		static const std::string ret =
				std::string( "Menu Root" )
			+	" | <" + r2tm::VersionInfo.String4Version + ">"
		;
		return ret.c_str();
	};
}
r2tm::DescriptionFunctionT Menu_Root::GetDescriptionFunction() const
{
	return []()->const char*
	{
		return "";
	};
}
r2tm::WriteFunctionT Menu_Root::GetWriteFunction() const
{
	return []( r2tm::MenuProcessor* mp )
	{
		mp->AddItem( '1', test_simdjson::Basic() );
		mp->AddItem( '2', test_simdjson::ErrorCode() );


		
		mp->AddLineFeed();



		mp->AddItem( 'q', test_simdjson::Parse_String() );
		mp->AddItem( 'w', test_simdjson::Parse_File() );
		mp->AddItem( 'e', test_simdjson::PaddedString() );



		mp->AddSplit();



		mp->AddExit( 27 );
	};
}