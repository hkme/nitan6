// This program is a part of NT MudLIB
// ab_crystal.c 能力水晶

#include <ansi.h>

inherit ITEM;

mapping dict = ([
        "axe"           :"斧",
        "bow"           :"弓",
        "sword"         :"剑",
        "blade"         :"刀",
        "club"          :"棍",
        "dagger"        :"匕首",
        "fork"          :"叉",
        "hammer"        :"锤",
        "staff"         :"杖",
        "throwing"      :"暗器",
        "whip"          :"鞭",
        "xsword"        :"箫",
        "head"          :"头盔",
        "neck"          :"项链",
        "cloth"         :"衣服",
        "charm"         :"符文",
        "rings"         :"戒指",
        "armor"         :"护甲",
        "surcoat"       :"披风",
        "waist"         :"腰带",
        "wrists"        :"护腕",
        "shield"        :"盾甲",
        "hands"         :"铁掌",
        "boots"         :"靴子",
        "finger"        :"指套",
        "all"           :"所有类型",
]);

string to_chinese(string str)
{
        if (! undefinedp(dict[str]))
                return dict[str];
        else
                return str;
}

void create()
{
        set_name(HIM"能力水晶"NOR, ({ "ability crystal", "ability", "crystal" }) );
        set_weight(1);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("long", HIM "表面泛着粉色光芒的能力水晶，可以将签名后的装备的镶嵌宝石属性、强化效果\n"
                            HIM "改造及套装效果储存(store)起来，然后可以无损失注入(infuse)到新的签名装备上。\n" NOR);
                set("value", 50000);
                set("unit", "块");
        }
        //set("set_data", 1);
        set("autoload", 1);
        setup();
}

void init()
{
        add_action("do_store", "store");
        add_action("do_infuse", "infuse");
}

int do_store(string arg)
{
        object me, ob;
        mapping magic, enchase, insert, apply, qianghua;
        string mod_name, mod_level;
        string type;
        int status;
        
        me = this_player();
        
        if( !arg || arg == "" )  
                return notify_fail("你要储存什么装备上的能力？\n");

        if( !objectp(ob = present(arg, me)) && 
            !objectp(ob = present(arg, environment(me)))) 
                return notify_fail("你身上和附近没有这样装备啊。\n"); 

        if( !ob->is_item_make() )
                return notify_fail("能力水晶只可存储已签名装备的能力。\n");

        if( !wizardp(me) && ob->item_owner() != query("id", me) )
                return notify_fail("这个，好象不属于你吧，这样不大合适。\n");

        if( query("equipped", ob) ) 
                return notify_fail("你先解除" + ob->name() + "的装备再说！\n"); 
                
        if( !mapp(enchase = query("enchase", ob)) || !mapp(insert=query("insert", ob)) )
                return notify_fail("能力水晶只能存储镶嵌过宝石的装备属性。\n");
        
        if( query("can_infuse") )
                return notify_fail("能力水晶已经储存过能力，无法再存储。\n");
                
        if( ob->is_weapon() )
                type = query("skill_type", ob);
        else
                type = query("armor_type", ob);
        
        set("can_infuse", type);
                
        status = query("status", ob); // 改造
        set("status", status);
        set("status", 1, ob); 
        delete("reform", ob);
        
        if( !undefinedp(enchase["mod_prop"]) ) // 套装
        {
                mod_name = query("mod_name", ob);
                mod_level = query("mod_level", ob);
                set("mod_name", mod_name);
                set("mod_level", mod_level);
                delete("mod_name", ob);
                delete("mod_level", ob);
                delete("mod_mark", ob);
        }
        
        set("enchase", enchase);      // 镶嵌
        set("insert", insert);
        delete("enchase", ob);
        delete("insert", ob);
        
        qianghua = query("qianghua", ob); // 强化
        if( mapp(qianghua) && sizeof(qianghua) > 0 )
        {
                set("qianghua", qianghua);
                delete("qianghua", ob);
        }

        magic = query("magic", ob); // 金木水火土属性
        if( mapp(magic) && sizeof(magic) > 0 )
        {
                set("magic", magic);
                delete("magic", ob);
        }
        
        set("set_data", 1);
        
        ob->save(); 
        
        tell_object(me, HIC "你将" +ob->name()+ HIC "的能力存储到能力水晶成功。\n" NOR);
        return 1;
}

int do_infuse(string arg)
{
        object me, ob;
        mapping magic, enchase, insert, apply, qianghua, props;
        string *key;
        string type;
        int i, value, flute;
        int status;

        me = this_player();
        
        if( !arg || arg == "" )  
                return notify_fail("你要给什么装备注入能力？\n");

        if( !objectp(ob = present(arg, me)) && 
            !objectp(ob = present(arg, environment(me)))) 
                return notify_fail("你身上和附近没有这样装备啊。\n"); 

        if( !ob->is_item_make() )
                return notify_fail("能力水晶只可给已签名装备注入能力。\n");

        if( !wizardp(me) && ob->item_owner() != query("id", me) ) 
                return notify_fail("这个，好象不属于你吧，这样不大合适。\n");

        if( query("equipped", ob) ) 
                return notify_fail("你先解除" + ob->name() + "的装备再说！\n"); 
                        
        enchase = query("enchase");
        if( !mapp(enchase) || sizeof(enchase) < 1 )
                return notify_fail("能力水晶没有储存过任何能力！\n");
        
        type = query("can_infuse");
        if(  type != query("skill_type", ob) && type != query("armor_type", ob) )
                return notify_fail("能力水晶只能给"+to_chinese(type)+"注入能力。\n");
        
        /*        
        flute = enchase["flute"];
        if( query("enchase/flute", ob) < flute )
                return notify_fail(ob->name()+"的已开孔数必须不小于能力水晶中存储能力的孔数("+flute+")！\n");
        */
        
        if( (status = query("status")) > 1 ) // 改造
        {
                set("status", status, ob);
                for( i=2;i<=status;i++ )
                {
                        addn("reform/apply_prop/max_jing", 1000*i, ob);     // 增加精
                        addn("reform/apply_prop/max_qi", 2000*i, ob);       // 增加气
                }
                
                if( type == "rings" || type == "neck" || type == "charm" )
                {
                        addn("reform/apply_prop/research_effect", status-1, ob);
                        addn("reform/apply_prop/derive_effect", status-1, ob);
                        addn("reform/apply_prop/practice_effect", status-1, ob);
                }
                if( query("status") == 6 )
                {
                        if( type == "rings" || type == "neck" || type == "charm" )
                        {
                                addn("reform/apply_prop/research_effect", 3, ob);
                                addn("reform/apply_prop/derive_effect", 3, ob);
                                addn("reform/apply_prop/practice_effect", 3, ob);
                        }
                
                        if( ob->is_weapon() )
                        {
                                props = query("weapon_prop", ob);
                                map_delete(props, "damage");
                        }
                        else 
                        {
                                props = query("armor_prop", ob);
                                map_delete(props, "unarmed_damage");
                                map_delete(props, "armor");
                        }
                        if( sizeof(props) > 0 )
                        {
                                key = keys(props);
                                for( i=0;i<sizeof(key);i++ )
                                {
                                        value = EQUIPMENT_D->query_prop_value(key[i], 3, 1); // 强化值
                                        if( query("quality_level", ob) == 4 )
                                        {
                                                if( sizeof(key) == 1 )
                                                        value = value * 3;
                                        } else
                                        if( query("quality_level", ob) == 5 )
                                        {
                                                if( sizeof(key) == 1 )
                                                        value = value * 4;
                                                else if( sizeof(key) == 2 )
                                                        value = value * 2;
                                        } else
                                        if( query("quality_level", ob) == 6 )
                                        {
                                                if( sizeof(key) == 1 )
                                                        value = value * 16;
                                                else if( sizeof(key) == 2 )
                                                        value = value * 8;
                                                else if( sizeof(key) == 3 )
                                                        value = value * 4;
                                                else
                                                        value = value * 2;
                                        }
                                        addn("reform/apply_prop/"+key[i], value, ob); 
                                }
                        }
                }
        }
                 
        set("enchase", enchase, ob);  // 镶嵌
        insert = query("insert");
        set("insert", insert, ob);
        qianghua = query("qianghua");   // 强化
        if( mapp(qianghua) && sizeof(qianghua) > 1 )
                set("qianghua", qianghua, ob);

        magic = query("magic");   // 金木水火土属性
        if( mapp(magic) && sizeof(magic) > 1 )
                set("magic", magic, ob);
        
        if( query("mod_name") )    // 套装
        {
                set("mod_name", query("mod_name"), ob);
                set("mod_mark", base_name(ob), ob);
        }

        if( query("mod_level") )
                set("mod_level", query("mod_level"), ob);
                                
        ob->save();
        
        tell_object(me, HIC "你将能力水晶中的能力注入" +ob->name()+ HIC "成功。\n" NOR);
        destruct(this_object());
        return 1;
}

string extra_long()
{
        mapping enchase, qianghua;
        string *apply, str, type;
        int value;
        int i;

        str =  sprintf( YEL "\n物品属性 : 特殊物<能力水晶>，重量%d克，下线%s丢失\n" NOR,
                        this_object()->query_weight(),
                        (this_object()->query_autoload() || query("autoload")) ? "不" : "" );

        if( mapp(enchase=copy(query("enchase/apply_prop"))) ) 
        {
                type = query("can_infuse");
                str += sprintf(HIK "附魔装备 : %s(%s)\n" NOR, to_chinese(type), type);
                apply = keys(enchase);
                for (i = 0; i<sizeof(apply); i++) 
                {
                        value = enchase[apply[i]];
                        str += HIK "拥有属性" NOR " : ";
                        str += HIB + EQUIPMENT_D->chinese(apply[i], value) + NOR + "\n";
                }    
        }

        if( mapp(enchase=query("enchase/mod_prop")) ) 
        {
                apply = keys(enchase);
                for (i = 0; i<sizeof(apply); i++) 
                {
                        value = enchase[apply[i]];
                        str += HIK "拥有属性" NOR " : ";
                        str += HIB + EQUIPMENT_D->chinese(apply[i], value) + NOR + "\n";
                }
        }
                
        if( mapp(qianghua=query("qianghua/apply_prop")) ) 
        {
                apply = keys(qianghua);
                for (i = 0; i<sizeof(apply); i++) 
                {
                        value = qianghua[apply[i]];
                        str += HIK "拥有属性" NOR " : ";
                        str += HIB + EQUIPMENT_D->chinese(apply[i], value) + NOR + "\n";
                }
        }
        
        return str;
}

string query_name() { return HIM"能力水晶"NOR; }
