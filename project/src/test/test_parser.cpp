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

			DECLARATION_MAIN( const simdjson::padded_string jsong_string = R"( [1, 2, 3, 4] )"_padded );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "∞·∞˙ µ•¿Ã≈Õ »πµÊ : µ•¿Ã≈Õ + error code" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( jsong_string ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				PROCESS_MAIN( result.get( datas ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "¿ﬂ∏¯µ» µ•¿Ã≈Õ" );

				LF();

				DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( R"( [ )"_padded ) );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "µ•¿Ã≈Õ »πµÊ" );

				LF();

				DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( jsong_string ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "ø°∑Ø ∏ﬁºº¡ˆøÕ µ•¿Ã≈Õ »πµÊ" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element datas );
				DECLARATION_MAIN( const simdjson::error_code error = p.parse( jsong_string ).get( datas ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );
				OUTPUT_VALUE( datas.type() );
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
				OUTPUT_SUBJECT( "∞·∞˙ µ•¿Ã≈Õ »πµÊ : µ•¿Ã≈Õ + error code" );

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
				OUTPUT_SUBJECT( "µ•¿Ã≈Õ »πµÊ" );

				LF();

				DECLARATION_MAIN( const simdjson::dom::element datas = p.load( path ) );
				OUTPUT_VALUE( datas.type() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "ø°∑Ø ∏ﬁºº¡ˆøÕ µ•¿Ã≈Õ »πµÊ" );

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