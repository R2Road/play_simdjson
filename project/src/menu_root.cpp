#include "menu_root.h"

#include <string>

#include "r2tm/r2tm_MenuProcessor.h"
#include "r2tm/r2tm_VersionInfo.h"

#include "test/test_basic.h"
#include "test/test_element.h"
#include "test/test_parser.h"

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
		return
					"    " "[o] " "읽기 기능 확인"
			"\n"	"    " "[~] " "쓰기 기능 확인 : 해당 기능이 미구현 상태"
			"\n"	"    " "  > " "이러면 쓸 수 없다.";
	};
}
r2tm::WriteFunctionT Menu_Root::GetWriteFunction() const
{
	return []( r2tm::MenuProcessor* mp )
	{
		mp->AddItem( '1', test_basic::Demo() );
		mp->AddItem( '2', test_basic::ResultType() );
		mp->AddItem( '3', test_basic::ErrorCode() );
		mp->AddItem( '4', test_basic::PaddedString() );


		
		mp->AddLineFeed();



		mp->AddItem( 'q', test_parser::Parse_String() );
		mp->AddItem( 'w', test_parser::Parse_File() );
		mp->AddItem( 'e', test_element::Element_Get() );
		mp->AddItem( 'r', test_element::Element_Array() );
		mp->AddItem( 't', test_element::Element_Object() );
		mp->AddItem( 'y', test_element::Element_Int() );



		mp->AddLineFeed();



		mp->AddMessage( "simdjson::fallback::ondemand::parser", r2tm::eColor::FG_LightGreen );
		mp->AddMessage( "스트리밍 방식으로 성능상에 이점이 있단다.", r2tm::eColor::FG_LightGreen );
		mp->AddItem( 'z', test_basic::Parse_Iterate() );



		mp->AddSplit();



		mp->AddExit( 27 );
	};
}