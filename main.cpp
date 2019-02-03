#include <iostream>
#include <algorithm>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "token_parser.h"
#include "css_parser.h"
#include "html_parser.h"
#include "html_node.h"
#include "query_parser.h"

static html_parser html;
static css_parser css;
void print(std::vector<std::wstring> tokens){
    std::cout << "==== TOKENS ====" << std::endl;
    for (std::wstring t : tokens) {
        std::wcout << L"\"" << t << L"\" ";
    }
    std::wcout << "================" << std::endl;
}

void print(std::string title, std::wstring text){
    std::cout << "==== " + title + " ====" << std::endl;
    std::wcout << text << std::endl;
    std::cout << "================" << std::endl;
};

void init_test() {
    auto html_text = LR"~(<!DOCTYPE HTML>
                     <html>
                     <head>
                     <meta name="qrichtext" content="1" />
                     <style type="text/css">
                     p, li { white-space: pre-wrap; }
                     b { color: "red" }
                     </style>
                     </head>
                     <body style=" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;">
                     <p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">dfg</p>
                     <img src="image.png" />
                     <p id="p1" class="par">
                     <b>Sample</b> paragraph متن فارسی
                     <img src="image.png" />
                     </p>
                     </body>
                     </html>)~";

    auto css_text = LR"~(
                    body{
                    color: red;
                    background-color: green;
                    }
                    body div{
                    display: block;
                    }
                    /*comment outside - /*/
            .p{ coloe: green; }
                    .p,div{
                    background-image: url('image.png');
                    padding: 2px;
                    border: 1px solid green;
                    /*comment: none*/color:red;
                    }

                    )~";

    html.set_text(html_text);
    css.set_text(css_text);
}
TEST_CASE( "Init", "[init]" ) {
    init_test();
}
TEST_CASE("String", "[string]") {
    print("HTML Formatted", html.root_tag()->to_string(print_type::formatted));
    print("HTML Compact", html.root_tag()->to_string(print_type::compact));
}

//TEST_CASE("Html", "[html]"){
//    auto tags = html.query(L"p.par b");
//    REQUIRE(1 == tags.size());
//}

//TEST_CASE("CSS Rules", "[css]") {
//    REQUIRE(4 == css.doc.size());
//    REQUIRE(1 == css.find_match_selector(L"body").size());
//    REQUIRE(2 == css.find_contains_selector(L".p").size());
//}


void debug() {
    print(html.tokens());
    print(css.tokens());
    print("HTML compact", html.to_string());
    print("HTML formatted", html.to_string(print_type::formatted));
    print("CSS compact", css.doc.to_string());
    print("CSS formatted", css.doc.to_string(print_type::formatted));
}

