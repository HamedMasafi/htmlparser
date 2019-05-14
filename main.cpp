#include <iostream>
#include <algorithm>
#include <json_object.h>

#include "include/json_value.h"
#include "include/token_parser.h"
#include "include/css_document.h"
#include "include/html_document.h"
#include "include/html_node.h"
#include "include/query_parser.h"
#include "include/json_document.h"

#define ASSERT(T) \
    if (!(T)) { \
    std::cout << "Assert faild on line: " << __LINE__ << " : (" << #T << ")" << std::endl; \
    exit(1); \
    }

#define PASSED() \
    std::cout << "All tests PASSED" << std::endl; \
    exit(0);

using namespace parser;

static html_document html;
static css_document css;
static json_document json;

void print(std::vector<std::string> tokens){
    std::cout << "==== TOKENS ====" << std::endl;
    for (std::string t : tokens) {
        std::cout << "\"" << t << "\" ";
    }
    std::cout << "================" << std::endl;
}

void print(std::string title, std::string text){
    std::cout << "==== " + title + " ====" << std::endl
              << text << std::endl
              << "================" << std::endl;
};

void init_test() {
    auto html_text = R"~(<!DOCTYPE HTML>
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
                     <b>Sample</b> paragraph
                     <img src="image.png" />
                     </p>
                     </body>
                     </html>)~";

    auto css_text = R"~(
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

    auto json_text = R"~(
                     {
                        name: hamed,
                        last_name: masafi,
                        unescaped: "the'string",
                        unescaped2: 'the"string',
                        unescaped3: "the'one\"string",
                        array: [1, 2, 3, 4, 5],
                        other: 4,
                        "f" : 3.14,
                        'n': 10,
                        ia: [
                                {a : 2}, {b: 23},
                                [1,2,5,7]
                            ]
                    }
                     )~";
    html.set_text(html_text);
    css.set_text(css_text);
    json.set_text(json_text);
}

int main() {

    init_test();

//    print("HTML Formatted", html.to_string(print_type::formatted));
//    print("HTML Compact", html.to_string(print_type::compact));
//    print("CSS formatted", css.to_string(print_type::formatted));
//    print("CSS compact", css.to_string(print_type::compact));
    print("JSON formatted", json.to_string(print_type::formatted));
    print("JSON compact", json.to_string(print_type::compact));

    auto tags = html.query("p.par>b");
    std::for_each(tags.begin(), tags.end(), [](html_node *tag){
        tag->to_tag()->add_class("new-class");
    });

    ASSERT(1 == tags.size());
    ASSERT(4 == css.nodes().size());
    ASSERT(1 == css.find_match_selector("body").size());
    ASSERT(2 == css.find_contains_selector(".p").size());
    auto v = json.find("ia.1.b");
    ASSERT(v != nullptr);
    ASSERT(v->type() == json_value::type_t::int_t);
    auto v2 = json.find("invalid_path");
    ASSERT(v2 == nullptr);
    PASSED();
}

