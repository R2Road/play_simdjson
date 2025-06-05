#include "Menu_SimdJson.h"

#include "r2tm/r2tm_MenuProcessor.h"

#include "test_simdjson.h"

#include "menu_root.h"

r2tm::TitleFunctionT Menu_SimdJson::GetTitleFunction() const
{
	return []()->const char*
	{
		return "SimdJson";
	};
}
r2tm::DescriptionFunctionT Menu_SimdJson::GetDescriptionFunction() const
{
	return []()->const char*
	{
		return
					"> Inprogress : ..."
			"\n"	"> ...";
	};
}
r2tm::WriteFunctionT Menu_SimdJson::GetWriteFunction() const
{
	return []( r2tm::MenuProcessor* mp )
	{
		mp->AddItem( '1', test_simdjson::Basic() );
		mp->AddItem( '2', test_simdjson::ErrorCode() );



		mp->AddSplit();



		mp->AddMenu( 27, Menu_Root() );
	};
}