#include "test_basic.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_basic
{
	r2tm::TitleFunctionT Basic::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "SimdJson : Basic";
		};
	}
	r2tm::DoFunctionT Basic::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );

			LS();

			DECLARATION_MAIN( const auto abstract_json = R"(
				[
					{  "12345" : {"a":12.34, "b":56.78, "c": 9998877}   },
					{  "12545" : {"a":11.44, "b":12.78, "c": 11111111}  }
				]
			)"_padded );

			LS();

			{
				OUTPUT_FILE_RANGE( __FILE__, 41, 52 );

				SS();

				for( simdjson::dom::object obj : parser.parse( abstract_json ) )
				{
					for( const auto& key_value : obj )
					{
						std::cout << "key: " << key_value.key << " : ";

						simdjson::dom::object innerobj = key_value.value;
						std::cout << "a: " << double( innerobj["a"] ) << ", ";
						std::cout << "b: " << double( innerobj["b"] ) << ", ";
						std::cout << "c: " << int64_t( innerobj["c"] ) << std::endl;
					}
				}
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT ResultType::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "ResultType";
		};
	}
	r2tm::DoFunctionT ResultType::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "��κ��� ��� simdjson �� �޼ҵ���� simdjson_result �� ��ȯ�Ѵ�" );
			OUTPUT_SUBJECT( "std::pair ���" );
			OUTPUT_SUBJECT( "error code + data" );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::simdjson_result result = p.parse( R"( [1] )"_padded ) );

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result::error() : ���� �ڵ� Ȯ��" );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result::get() : �� ���� + ���� �ڵ� ��ȯ" );

				LF();

				DECLARATION_MAIN( simdjson::dom::array a );
				EXPECT_EQ( simdjson::error_code::SUCCESS, result.get( a ) );

				LF();

				EXPECT_EQ( 1, a.size() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "simdjson_result �� �ʿ� ���ٸ� �ٷ� �� ����" );

				LF();

				DECLARATION_MAIN( simdjson::dom::array a = p.parse( R"( [1] )"_padded ) );

				LF();

				EXPECT_EQ( 1, a.size() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT ErrorCode::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Error Code";
		};
	}
	r2tm::DoFunctionT ErrorCode::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "�޼ҵ��� �۵� ����� simdjson::error_code �� Ȯ��." );
			OUTPUT_SUBJECT( "simdjson::error_code�� �����ϴ� operator<< �� ���� �Ѵ�." );
			OUTPUT_SUBJECT( "�̸� �غ�� error message �� Ȯ�� �����ϴ�." );

			LS();

			DECLARATION_MAIN( simdjson::dom::parser parser );

			LS();

			{
				OUTPUT_SUBJECT( "�� ���ڿ�" );

				LF();

				DECLARATION_MAIN( const auto s = R"()"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::EMPTY, e );
				OUTPUT_VALUE( simdjson::error_code::EMPTY );
			}

			LS();

			{
				OUTPUT_SUBJECT( "�Ϸ� ���� ���� ������ ǥ��" );

				LF();

				DECLARATION_MAIN( const auto s = R"( [ )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::TAPE_ERROR, e );
				OUTPUT_VALUE( simdjson::error_code::TAPE_ERROR );
			}

			LS();

			{
				OUTPUT_SUBJECT( "���� ������" );

				LF();

				DECLARATION_MAIN( const auto s = R"( [] )"_padded );

				LF();

				DECLARATION_MAIN( const simdjson::error_code e = parser.parse( s ).error() );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, e );
				OUTPUT_VALUE( simdjson::error_code::SUCCESS );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT PaddedString::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Padded String";
		};
	}
	r2tm::DoFunctionT PaddedString::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "parser�� �۵��� �����ϰ� ���ִ� ���ڿ� Ÿ��" );
			OUTPUT_SUBJECT( "...�̶���." );

			LS();

			{
				OUTPUT_SUBJECT( "�⺻ ������" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s );

				LF();

				EXPECT_EQ( 0, s.length() );
				EXPECT_EQ( 0, s.size() );
				EXPECT_EQ( nullptr, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "char*" );

				LF();

				DECLARATION_MAIN( const char* c = "123" );
				DECLARATION_MAIN( const simdjson::padded_string s( c, strlen( c ) ) );

				LF();

				EXPECT_EQ( strlen( c ), s.length() );
				EXPECT_EQ( strlen( c ), s.size() );
				EXPECT_NE( c, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "operator \"\"_padded" );

				LF();

				DECLARATION_MAIN( const simdjson::padded_string s = R"( [] )"_padded );

				LF();

				EXPECT_NE( nullptr, s.data() );
			}

			LS();

			{
				OUTPUT_SUBJECT( "File" );

				LF();

				DECLARATION_MAIN( const char* f = "resource/json_basic.json" );
				DECLARATION_MAIN( simdjson::simdjson_result result = simdjson::padded_string::load( f ) );
				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				OUTPUT_VALUE( result.value() );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}



	r2tm::TitleFunctionT Parse_Iterate::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Parse : Iterate";
		};
	}
	r2tm::DoFunctionT Parse_Iterate::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			DECLARATION_MAIN( const char* const path = "resource/json_from_plp.json" );

			SS();

			DECLARATION_MAIN( simdjson::padded_string s = simdjson::padded_string::load( path ) );

			LS();

			DECLARATION_MAIN( simdjson::fallback::ondemand::parser p );

			LS();

			{
				OUTPUT_SUBJECT( "��� ������ ȹ�� : ������ + error code" );

				LF();

				DECLARATION_MAIN( simdjson::simdjson_result result = p.iterate( s ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, result.error() );

				LF();

				OUTPUT_NOTE( "������ ������ ���� �𸣰ھ�" );
				OUTPUT_NOTE( "�ȵǰڴ� �ϴ� ����. 2025.06.06" );
			}

			LS();

			{
				OUTPUT_SUBJECT( "������ ȹ��" );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::document doc = p.iterate( s ) );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::array ary = doc.get_array() );
				DECLARATION_MAIN( simdjson::fallback::ondemand::object obj = ary.at( 0 ) );
				OUTPUT_VALUE( obj["idx"] );
			}

			LS();

			{
				OUTPUT_SUBJECT( "���� �޼����� ������ ȹ��" );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::document doc );
				DECLARATION_MAIN( simdjson::error_code error = p.iterate( s ).get( doc ) );

				LF();

				EXPECT_EQ( simdjson::error_code::SUCCESS, error );

				LF();

				DECLARATION_MAIN( simdjson::fallback::ondemand::array ary = doc.get_array() );
				DECLARATION_MAIN( simdjson::fallback::ondemand::object obj = ary.at( 0 ) );
				OUTPUT_VALUE( obj["idx"] );
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}