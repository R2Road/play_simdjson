#include "menu_root.h"

#include <string>

#include "r2tm/r2tm_MenuProcessor.h"
#include "r2tm/r2tm_VersionInfo.h"

#include "test/test_element.h"
#include "test/test_parser.h"
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
		mp->AddItem( '3', test_simdjson::PaddedString() );


		
		mp->AddLineFeed();



		mp->AddItem( 'q', test_parser::Parse_String() );
		mp->AddItem( 'w', test_parser::Parse_File() );
		mp->AddItem( 'e', test_element::Element_Array() );
		mp->AddItem( 'r', test_element::Element_Object() );



		mp->AddLineFeed();



		mp->AddMessage( "simdjson::fallback::ondemand::parser", r2tm::eColor::FG_LightGreen );
		mp->AddMessage( "스트리밍 방식으로 성능상에 이점이 있단다.", r2tm::eColor::FG_LightGreen );
		mp->AddItem( 'z', test_simdjson::Parse_Iterate() );



		mp->AddSplit();



		mp->AddExit( 27 );
	};
}