#include <ansi.h>

object book = find_object("/clone/book/qiankun_book");

object get_owner()
{
        object owner;
        if(objectp(book)) {
                owner = environment(book);
                if( !objectp(owner) ) return 0;
                if(owner->is_character()) return owner;
        }
        return 0;
}

object book_owner = get_owner();

string get_book()
{
        if(objectp(book)) destruct(book);
        if(objectp(book_owner) && userp(book_owner)) book_owner->unconcious();
        return "张无忌：...............去死吧！";
}

nosave string book_owner_name = objectp(book_owner)?query("name", book_owner):"某人";

nosave mixed *story = ({
        "明教护教使者：报……，听说乾坤心法被外人偷走了……。",
        "张无忌：咦……他奶奶的，敢到我明教偷东西，得好好找找！",
        "明教上下开始全面寻找此物......",
        "张无忌道：怎么找不到了呢？有谁看到是谁偷的了么？",
        "明教护教使者：报……好象是给" + book_owner_name + "偷去了。",
        "张无忌道：靠！怎么不早说？我现在下山就去找他",
        "张无忌来到" + book_owner_name + "面前。",
        book_owner_name + "拔腿想跑，可被逮了个正着。",
                "张无忌：%—*（）（—……*—%￥###￥*（）（%*",
                "张无忌：你这小子！居然敢偷我明教的乾坤心法。",
        "张无忌使出九阳乾坤无上心法，" + book_owner_name + "顿时被吓的半死！",
        book_owner_name + "：切！不就是乾坤心法么，大不了还给你。",
        book_owner_name + "掏出羊皮书丢给张无忌。",
        (: get_book :),
        book_owner_name + "被张无忌打晕了。",
        "张无忌开开心心的回到明教。",
});

string prompt() { return HIW "【武林轶事】" NOR; }

void create()
{
        seteuid(getuid());
}

mixed query_story_message(int step)
{
        return step < sizeof(story) ? story[step] : 0;
}
