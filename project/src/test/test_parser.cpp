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

			DECLARATION_MAIN( const char* const path = "resource/json_from_plp.json" );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "결과 데이터 획득 : 데이터 + error code" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.load( path ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				PROCESS_MAIN( result.get( datas ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "데이터 획득" );

				LF();

				DECLARATION_MAIN( const simdjson::dom::element datas = p.load( path ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "에러 메세지와 데이터 획득" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				DECLARATION_MAIN( const simdjson::error_code error = p.load( path ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}