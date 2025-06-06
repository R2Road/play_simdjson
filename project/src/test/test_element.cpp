#include "test_element.h"

#include "r2tm/r2tm_Inspector.h"
#include "r2tm/r2tm_ostream.h"

#include "simdjson/simdjson.h"

namespace test_element
{
	r2tm::TitleFunctionT Element_Array::GetTitleFunction() const
	{
		return []()->const char*
		{
			return "Element : Array";
		};
	}
	r2tm::DoFunctionT Element_Array::GetDoFunction() const
	{
		return []()->r2tm::eDoLeaveAction
		{
			LS();

			OUTPUT_SUBJECT( "Array" );

			LS();

			DECLARATION_MAIN( const auto s = R"( [0, 1, 2, 3] )"_padded );
			DECLARATION_MAIN( simdjson::dom::parser p );
			DECLARATION_MAIN( const simdjson::dom::element datas = p.parse( s ) );

			LS();

			DECLARATION_MAIN( simdjson::dom::array a = datas.get_array() );
			EXPECT_EQ( 4, a.size() );

			LS();

			{
				OUTPUT_SUBJECT( "[] 안된다." );

				LF();

				OUTPUT_SOURCE_READY_N_BEGIN;
				// a[1];
				OUTPUT_SOURCE_END;
			}

			LS();

			{
				OUTPUT_SUBJECT( "at" );

				LF();

				DECLARATION_MAIN( simdjson::dom::element at_3 = a.at( 3 ) );
				EXPECT_EQ( 3, at_3.get_int64() );

				LF();

				OUTPUT_COMMENT( "element 에서 바로 형변환." );
				EXPECT_EQ( 3, int64_t( a.at( 3 ) ) );
			}

			LS();

			{
				OUTPUT_SUBJECT( "ranged-for" );

				LF();

				OUTPUT_SOURCE_READY_N_BEGIN;
				for( const auto i : a ) { std::cout << i << "   "; }
				LF();
				OUTPUT_SOURCE_END;
			}

			LS();

			return r2tm::eDoLeaveAction::Pause;
		};
	}
}