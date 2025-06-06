#include "test_parser.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_parser
{
	r2tm::TitleFunctionT Parse_String::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : String";
		};
	}
	r2tm::DoFunctionT Parse_String::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "정상 데이터" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s = R"( [1, 2, 3, 4] )"_padded );
				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( s ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "잘못된 데이터" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s = R"( [ )"_padded );
				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( s ) );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, result.error() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Parse_File::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : File";
		};
	}
	r2tm::DoFunctionT Parse_File::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "정상 데이터" );

				LF();

				DECLARATION_MAIN( const char* const path = "resource/json_basic.json" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.load( path ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "잘못된 데이터" );

				LF();

				DECLARATION_MAIN( const char* const path = "resource/json_wrong.json" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.load( path ) );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, result.error() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}