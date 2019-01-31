#include <cssparser.h>
#include <tokenparser.h>
#include <iostream>

#include "htmlparser.h"
#include "htmltag.h"

int main(int argc, char *argv[])
{

    auto html = LR"~(<!DOCTYPE HTML>
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
<p id="p1">
<b>Sample</b> paragraph
<img src="image.png" />
</p>
</body>
</html>)~";

    auto css = LR"~(
body{
color: red;
background-color: green;
}
body div{
    display: block;
}
/*comment outside - /*/
.p,div{
background-image: url('image.png');
padding: 2px;
                                    border: 1px solid green;
/*comment: none*/color:red;
}

)~";

    auto print_list = [](std::vector<std::wstring> tokens){
        std::wcout << "==== TOKENS ====" << std::endl;
        for (std::wstring t : tokens) {
            std::wcout << L"\"" << t << L"\" ";
        }
        std::wcout << L"================" << std::endl;
//        std::wcout << std::flush;
    };
    auto print_string = [](std::wstring title, std::wstring text){
        std::wcout << L"==== " + title + L" ====" << std::endl;
        std::wcout << text << std::endl;
        std::wcout << "================" << std::endl;
//        std::wcout << std::flush;
    };

    html_parser tp;
    tp.setText(html);
    tp.parse();

    css_parser cp;
    cp.setText(css);
    cp.parse();

//    print_list(tp._tokens);
//    print_list(cp._tokens);

    auto p1 = tp.get_by_id(L"p1");
    p1->set_attr(L"data-extra", L"new-name");
    p1->add_class(L"cls");

//    print_string(L"HTML compact", tp.to_string());
    print_string(L"HTML formatted", tp.to_string(print_type::formatted));
//    print_string(L"CSS compact", cp.doc.to_string());
//    print_string(L"CSS formatted", cp.doc.to_string(print_type::formatted));

    std::wcout << "END" << std::endl;
    std::wcout << std::flush;

    return EXIT_SUCCESS;
}
