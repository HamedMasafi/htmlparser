#include <iostream>
#include <algorithm>

#include "include/token_parser.h"
#include "include/css_parser.h"
#include "include/html_parser.h"
#include "include/html_node.h"
#include "include/query_parser.h"

#define ASSERT(T) \
    if (!(T)) \
    std::cout << "Assert faild on line: " << __LINE__ << " : (" << #T << ")" << std::endl;

static html_parser html;
static css_parser css;
void print(std::vector<std::string> tokens){
    std::cout << "==== TOKENS ====" << std::endl;
    for (std::string t : tokens) {
        std::cout << "\"" << t << "\" ";
    }
    std::cout << "================" << std::endl;
}

void print(std::string title, std::string text){
    std::cout << "==== " + title + " ====" << std::endl;
    std::cout << text << std::endl;
    std::cout << "================" << std::endl;
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

    html.set_text(html_text);
    css.set_text(css_text);
}

int main() {

    init_test();

    print("HTML Formatted", html.to_string(print_type::formatted));
    print("HTML Compact", html.to_string(print_type::compact));
    print("CSS compact", css.to_string(print_type::compact));
    print("CSS formatted", css.to_string(print_type::formatted));

    auto tags = html.query("p.par b");
    std::for_each(tags.begin(), tags.end(), [](html_tag *tag){
        tag->add_class("new-class");
    });
    ASSERT(1 == tags.size());
    ASSERT(4 == css.nodes().size());
    ASSERT(1 == css.find_match_selector("body").size());
    ASSERT(2 == css.find_contains_selector(".p").size());
}

