// This program is a part of NT MudLIB
// Written by Lonely@nitan.org
// combatd.c

#pragma optimize
#pragma save_binary

#include <ansi.h>
#include <skill.h>
#include <weapon.h>
#include <combat.h>

inherit F_DBASE;

#define NCH_CHANNEL(x) CHANNEL_D->channel_broadcast("nch", "战斗精灵："+(string)x) 

#define DEFAULT_MAX_PK_PERDAY   8
#define DEFAULT_MAX_PK_PERMAN   3
#define EXP_LIMIT       50000000000

#define ATTACK          0
#define DODGE           1
#define PARRY           2
#define DAMAGE          3
#define FORCE2          4

#undef REDUCE_PVP_DAMAGE

#define PVP_DAMAGE_SCALE        40;  // 玩家对玩家时伤害（当前值）下降为 40%
#define PVP_WOUND_SCALE         30;  // 玩家对玩家时伤害（最大值）下降为 30%

mapping limb_damage = ([
// 人类身体部位
        "头部"  :   100,
        "颈部"  :   90,
        "胸口"  :   90,
        "后心"  :   80,
        "左肩"  :   50,
        "右肩"  :   55,
        "左臂"  :   40,
        "右臂"  :   45,
        "左手"  :   20,
        "右手"  :   30,
        "腰间"  :   60,
        "小腹"  :   70,
        "左腿"  :   40,
        "右腿"  :   50,
        "左脚"  :   35,
        "右脚"  :   40,
//  动物身体部位
        "身体"  :   80,
        "前脚"  :   40,
        "后脚"  :   50,
        "腿部"  :   40,
        "尾巴"  :   10,
        "翅膀"  :   50,
        "爪子"  :   40,
]);

nosave string *danger_limbs = ({
        "头部", "颈部", "胸口", "后心","小腹",
});

string *head_damage_me_msg = ({
        append_color(HIR "你觉得头有一点发晕。\n" NOR, HIR),
        append_color(HIB "你觉得一阵晕眩，身体晃了一下。\n" NOR, HIB),
        append_color(BLU "你只得眼冒金星，浑身发飘。\n" NOR, BLU),
});

string *body_damage_me_msg = ({
        append_color(HIR "你只觉$l隐隐作痛，体内真气受到一点振荡。\n" NOR, HIR),
        append_color(HIB "你只觉$l生疼，体内真气一阵乱窜。\n" NOR, HIB),
        append_color(BLU "你只觉得$l处剧痛，连带着体内真气一阵外泄。\n" NOR, BLU),
});

string *guard_msg = ({
        append_color(CYN "$N注视著$n的行动，企图寻找机会出手。\n" NOR, CYN),
        append_color(CYN "$N正盯著$n的一举一动，随时准备发动攻势。\n" NOR, CYN),
        append_color(CYN "$N缓缓地移动脚步，想要找出$n的破绽。\n" NOR, CYN),
        append_color(CYN "$N目不转睛地盯著$n的动作，寻找进攻的最佳时机。\n" NOR, CYN),
        append_color(CYN "$N慢慢地移动著脚步，伺机出手。\n" NOR, CYN),
});

string *catch_hunt_human_msg = ({
        append_color(HIW "$N和$n仇人相见份外眼红，立刻打了起来！\n" NOR, HIW),
        append_color(HIW "$N对著$n大喝：「可恶，又是你！」\n" NOR, HIW),
        append_color(HIW "$N和$n一碰面，二话不说就打了起来！\n" NOR, HIW),
        append_color(HIW "$N一眼瞥见$n，「哼」的一声冲了过来！\n" NOR, HIW),
        append_color(HIW "$N一见到$n，愣了一愣，大叫：「我宰了你！」\n" NOR, HIW),
        append_color(HIW "$N喝道：「$n，我们的帐还没算完，看招！」\n" NOR, HIW),
        append_color(HIW "$N喝道：「$n，看招！」\n" NOR, HIW),
});

string *catch_hunt_beast_msg = ({
        append_color(HIW "$N怒吼一声，扑了过来攻击$n！\n" NOR, HIW),
        append_color(HIW "$N对著$n大吼，想杀死$n！\n" NOR, HIW),
        append_color(HIW "$N发出连串低吼，突然暴起攻击$n！\n" NOR, HIW),
});

string *catch_hunt_bird_msg = ({
        append_color(HIW "$N对著$n一声怒鸣，飞冲了过来！\n" NOR, HIW),
        append_color(HIW "$N怒鸣几声，突然暴起攻击$n！\n" NOR, HIW),
        append_color(HIW "$N一声锐鸣，猛然向$n发动攻击！\n" NOR, HIW),
});

string *catch_hunt_msg = ({
        append_color(HIW "$N和$n仇人相见分外眼红，立刻打了起来！\n" NOR, HIW),
        append_color(HIW "$N对著$n大喝：「可恶，又是你！」\n" NOR, HIW),
        append_color(HIW "$N和$n一碰面，二话不说就打了起来！\n" NOR, HIW),
        append_color(HIW "$N一眼瞥见$n，「哼」的一声冲了过来！\n" NOR, HIW),
        append_color(HIW "$N一见到$n，愣了一愣，大叫：「我宰了你！」\n" NOR, HIW),
        append_color(HIW "$N喝道：「$n，我们的帐还没算完，看招！」\n" NOR, HIW),
        append_color(HIW "$N喝道：「$n，看招！」\n" NOR, HIW),
});

string *winner_msg = ({
        append_color(CYN "\n$N哈哈大笑，说道：承让了！\n\n" NOR, CYN),
        append_color(CYN "\n$N双手一拱，笑著说道：承让！\n\n" NOR, CYN),
        append_color(CYN "\n$N胜了这招，向后跃开三尺，笑道：承让！\n\n" NOR, CYN),
        append_color(CYN "\n$N双手一拱，笑著说道：知道我的利害了吧！\n\n" NOR, CYN),
        append_color(CYN "\n$N胜了这招，向后跃开三尺，叹道：真是拔剑四顾心茫然！\n\n" NOR, CYN),
        append_color(CYN "\n$n向后退了几步，说道：这场比试算我输了，下回看我怎么收拾你！\n\n" NOR, CYN),
        append_color(CYN "\n$n向后一纵，恨恨地说道：君子报仇，十年不晚！\n\n" NOR, CYN),
        append_color(CYN "\n$n脸色一寒，说道：算了算了，就当是我让你吧！\n\n" NOR, CYN),
        append_color(CYN "\n$n纵声而笑，叫道：“你运气好！你运气好！”一面身子向后跳开。\n\n" NOR, CYN),
        append_color(CYN "\n$n脸色微变，说道：佩服，佩服！\n\n" NOR, CYN),
        append_color(CYN "\n$n向后退了几步，说道：这场比试算我输了，佩服，佩服！\n\n" NOR, CYN),
        append_color(CYN "\n$n向后一纵，躬身做揖说道：阁下武艺不凡，果然高明！\n\n" NOR, CYN),
});

string *weapon_parry_throwing = ({
        append_color(HIR "$n手中$v一挥，将$w打飞。\n" NOR, HIR),
        append_color(HIR "$n将$v在身前一横，$w打在$v上，$n手上觉得一阵酸麻。\n" NOR, HIR),
        append_color(HIR "$n猛力挥动手中$v，将$w打得无影无踪。\n" NOR, HIY),
});

string *unarm_parry_throwing = ({
        append_color(HIR "$n长袖一卷，将$w收入囊中。\n" NOR, HIY),
        append_color(HIR "$n伸出两个手指轻轻一夹，便将$w接在手中。\n" NOR, HIY),
        append_color(HIR "$n脱下一只鞋子对准$w一捞，$w掉入$n的鞋中。\n" NOR, HIY),
});

string *weapon_parry_sixmai = ({
        append_color(HIR "$n舞动$v，将自身罩在一道光幕之中，挡住了$N六脉神剑的攻势。\n" NOR, HIY),
        append_color(HIR "$n挥动$v，接住了$N六脉神剑的无形剑气。\n" NOR, HIY),
        append_color(HIR "$n挥舞$v，六脉神剑的无形剑气尽数打在$v上。\n" NOR, HIY),
});

string *unarm_parry_sixmai = ({
        append_color(HIR "$n运起内力，衣袖鼓风而前，接住了$N的六脉神剑，袖子却被无形剑气切下一片。\n" NOR, HIY),
        append_color(HIR "$n在身前布起一道气墙，只听「嗤」，「嗤」之声，$N的剑气尽数打在气墙上。\n" NOR, HIY),
        append_color(HIR "$n将内力尽数布在$l处，接下了$N的六脉神剑，但$l处的表皮却已烧焦。\n" NOR, HIY),
});

string *star_move_weapon = ({
        append_color(HIR "只见$n长袖在$N的$w上一带一送，$w在空中硬生生转了个湾，奔向$P自己的$l。\n" NOR, HIY),
        append_color(HIR "$n使一招斗转星移，$N的$w竟然倒转过来，奔向$P自己的$l。\n" NOR, HIY),
});

string *star_move_unarm = ({
        append_color(HIR "$n运用斗转星移，暗中将$N的内力尽数送了回去，$N只觉得$l处一阵剧疼。\n" NOR, HIY),
        append_color(HIR "$n的长袖一带一送，$N的这一招竟然打到了自己身上。\n" NOR, HIY),
});

string *qiankun_move_weapon = ({
        append_color(HIR "只见$n长袖在$N的$w上一带一送，$w在空中硬生生转了个湾，奔向$P自己的$l。\n" NOR, HIY),
        append_color(HIR "$n使一招乾坤大挪移，$N的$w竟然倒转过来，奔向$P自己的$l。\n" NOR, HIY),
});

string *qiankun_move_unarm = ({
        append_color(HIR "$n运用乾坤大挪移，暗中将$N的内力尽数送了回去，$N只觉得$l处一阵剧疼。\n" NOR, HIY),
        append_color(HIR "$n的长袖一带一送，$N的这一招竟然打到了自己身上。\n" NOR, HIY),
});

// message after damage info
nosave string foo_before_hit = 0;
nosave string foo_after_hit = 0;
string query_bhinfo() { return foo_before_hit; }
string query_ahinfo() { return foo_after_hit; }
void clear_bhinfo() { foo_before_hit = 0; }
void clear_ahinfo() { foo_after_hit = 0; }
protected void quest_kill(object killer, object victim);

void create()
{
        int pd, pm;

        seteuid(getuid());
        set("channel_id", "战斗精灵");

        if( undefinedp(pd = CONFIG_D->query_int("max pk perday")) )
                pd = DEFAULT_MAX_PK_PERDAY;

        if( undefinedp(pm = CONFIG_D->query_int("max pk perman")) )
                pm = DEFAULT_MAX_PK_PERMAN;

        set("pk_perday", pd);
        set("pk_perman", pm);
}

void set_bhinfo(string msg)
{
        if( !foo_before_hit ) {
                foo_before_hit = msg;
                return;
        }
        foo_before_hit += msg;
}

void set_ahinfo(string msg)
{
        if( !foo_after_hit ) {
                foo_after_hit = msg;
                return;
        }
        foo_after_hit += msg;
}

string damage_msg(int damage, string type)
{
        string str;

        if( damage == 0 )
                return "结果没有造成任何伤害。\n";

        switch( type )
        {
        case "擦伤":
        case "拉伤":
        case "割伤":
                if( damage < 100 ) return "结果只是轻轻地划破$p的皮肉。\n"; else
                if( damage < 200 ) return "结果在$p$l划出一道细长的血痕。\n"; else
                if( damage < 400 ) return "结果「嗤」地一声，$w已在$p$l划出一道伤口！\n"; else
                if( damage < 600 ) return "结果「嗤」地一声，$w已在$p$l划出一道血淋淋的伤口！\n"; else
                if( damage < 1000 )return "结果「嗤」地一声，$w已在$p$l划出一道又长又深的伤口，溅得$N满脸鲜血！\n"; else
                                   return "结果只听见$n一声惨嚎，$w已在$p$l划出一道深及见骨的可怕伤口！\n";
                break;
        case "刺伤":
                if( damage < 100 ) return "结果只是轻轻地刺破$p的皮肉。\n"; else
                if( damage < 200 ) return "结果在$p$l刺出一个创口。\n"; else
                if( damage < 400 ) return "结果「噗」地一声，$w已刺入了$n$l寸许！\n"; else
                if( damage < 800 ) return "结果「噗」地一声，$w已刺进$n的$l，使$p不由自主地退了几步！\n"; else
                if( damage < 1000 )return "结果「噗嗤」地一声，$w已在$p$l刺出一个血肉□糊的血窟窿！\n"; else
                                   return "结果只听见$n一声惨嚎，$w已在$p的$l对穿而出，鲜血溅得满地！\n";
                break;
        case "踢伤":
                if( damage < 50 )  return  "结果只是轻轻地踢到，比拍苍蝇稍微重了点。\n";else
                if( damage < 100 ) return "结果在$n的$l造成一处瘀青。\n";else
                if( damage < 200 ) return "结果一踢命中，$n的$l登时肿了一块老高！\n";else
                if( damage < 400 ) return "结果一踢命中，$n闷哼了一声显然吃了不小的亏！\n";else
                if( damage < 600 ) return "结果「砰」地一声，$n退了两步！\n";else
                if( damage < 800 ) return "结果这一下「砰」地一声踢得$n连退了好几步，差一点摔倒！\n";else
                if( damage < 1000 )return "结果重重地踢中，$n「哇」地一声吐出一口鲜血！\n";else
                                   return "结果只听见「砰」地一声巨响，$n像一捆稻草般飞了出去！！\n";
                break;
        case "瘀伤":
        case "震伤":
                if( damage < 50 )  return "结果只是轻轻地碰到，比拍苍蝇稍微重了点。\n"; else
                if( damage < 100 ) return "结果在$p的$l造成一处瘀青。\n"; else
                if( damage < 200 ) return "结果一击命中，$n的$l登时肿了一块老高！\n"; else
                if( damage < 400 ) return "结果一击命中，$n闷哼了一声显然吃了不小的亏！\n"; else
                if( damage < 600 ) return "结果「砰」地一声，$n退了两步！\n"; else
                if( damage < 800 ) return "结果这一下「砰」地一声打得$n连退了好几步，差一点摔倒！\n"; else
                if( damage < 1000 )return "结果重重地击中，$n「哇」地一声吐出一口鲜血！\n"; else
                                   return "结果只听见「砰」地一声巨响，$n像一捆稻草般飞了出去！\n";
                break;
        case "抓伤":
                if( damage < 50 )  return "结果只是轻轻地抓到，比抓痒稍微重了点。\n";else
                if( damage < 100 ) return "结果在$p的$l抓出几条血痕。\n";else
                if( damage < 200 ) return "结果一抓命中，$n的$l被抓得鲜血飞溅！\n";else
                if( damage < 400 ) return "结果「嗤」地一声，$l被抓得深可见骨！！\n";else
                if( damage < 600 ) return "结果一抓命中，$n的$l被抓得血肉横飞！！！\n";else
                if( damage < 800 ) return "结果这一下「嗤」地一声抓得$n连晃好几下，差一点摔倒！\n";else
                if( damage < 1000 )return "结果$n哀号一声，$l被抓得筋断骨折！！\n";else
                                   return "结果只听见$n一声惨嚎，$l被抓出五个血窟窿！鲜血溅得满地！！\n";
                break;
        case "内伤":
                if( damage < 50 )  return "结果只是把$n打得退了半步，毫发无损。\n"; else
                if( damage < 100 ) return "结果$n痛哼一声，在$p的$l造成一处瘀伤。\n"; else
                if( damage < 200 ) return "结果一击命中，把$n打得痛得弯下腰去！\n"; else
                if( damage < 400 ) return "结果$n闷哼了一声，脸上一阵青一阵白，显然受了点内伤！\n"; else
                if( damage < 600 ) return "结果$n脸色一下变得惨白，昏昏沉沉接连退了好几步！\n"; else
                if( damage < 800 ) return "结果重重地击中，$n「哇」地一声吐出一口鲜血！\n"; else
                if( damage < 1000 )return "结果「轰」地一声，$n全身气血倒流，口中鲜血狂喷而出！\n"; else
                                   return "结果只听见几声喀喀轻响，$n一声惨叫，像滩软泥般塌了下去！\n";
                break;
        case "点穴":
                if( damage < 60 )  return "结果只是轻轻的碰到$n的$l，根本没有点到穴道。\n"; else
                if( damage < 100 ) return "结果$n痛哼一声，在$p的$l造成一处淤青。\n"; else
                if( damage < 200 ) return "结果一击命中，$N点中了$n$l上的穴道，$n只觉一阵麻木！\n"; else
                if( damage < 400 ) return "结果$n闷哼了一声，脸上一阵青一阵白，登时觉得$l麻木！\n"; else
                if( damage < 800 ) return "结果$n脸色一下变得惨白，被$N点中$l的穴道,一阵疼痛遍布整个$l！\n"; else
                if( damage < 1000 )return "结果$n一声大叫，$l的穴道被点中,疼痛直入心肺！\n"; else
                                   return "结果只听见$n一声惨叫，一阵剧痛夹杂着麻痒游遍全身，跟着直挺挺的倒了下去！\n";
                break;
        case "抽伤":
        case "鞭伤":
                if( damage < 50 )  return "结果只是在$n的皮肉上碰了碰，好象只蹭破点皮。\n"; else
                if( damage < 100 ) return "结果在$n$l抽出一道轻微的紫痕。\n"; else
                if( damage < 200 ) return "结果「啪」地一声在$n$l抽出一道长长的血痕！\n"; else
                if( damage < 400 ) return "结果只听「啪」地一声，$n连晃好几下，差一点摔倒！\n"; else
                if (damage < 600)  return "结果$p的$l上被抽了一道血淋淋的创口！\n"; else
                if( damage < 800 ) return "结果只听「啪」地一声，$n的$l被抽得皮开肉绽，痛得$p咬牙切齿！\n"; else
                if( damage < 1000 )return "结果「啪」地一声爆响！这一下好厉害，只抽得$n皮开肉绽，血花飞溅！\n"; else
                                   return "结果只听见$n一声惨嚎，$w重重地抽上了$p的$l，$n顿时血肉横飞，十命断了九条！\n";
                break;
        case "反震伤":
        case "反弹伤":
                if( damage < 50 )  return "结果$N受到$n的内力反震，闷哼一声。\n"; else
                if( damage < 100 ) return "结果$N被$n的反震得气血翻腾，大惊失色。\n"; else
                if( damage < 200 ) return "结果$N被$n的反震得站立不稳，摇摇晃晃。\n"; else
                if( damage < 400 ) return "结果$N被$n以内力反震，「嘿」地一声退了两步。\n"; else
                if( damage < 600 ) return "结果$N被$n的震得反弹回来的力量震得半身发麻。\n"; else
                if( damage < 800 ) return "结果$N被$n的内力反震，胸口有如受到一记重击，连退了五六步！\n"; else
                if( damage < 1000 )return "结果$N被$n内力反震，眼前一黑，身子向後飞出丈许！\n"; else
                                   return "结果$N被$n内力反震，眼前一黑，狂吐鲜血，身子象断了线的风筝向後飞去！\n";
                break;
        case "砸伤":
        case "挫伤":
                if( damage < 50 )  return "结果只是轻轻地碰到，像是给$n搔了一下痒。\n"; else
                if( damage < 100 ) return "结果在$n的$l砸出一个小臌包。\n"; else
                if( damage < 200 ) return "结果$N这一下砸个正着，$n的$l登时肿了一块老高！\n"; else
                if( damage < 400 ) return "结果$N这一下砸个正着，$n闷哼一声显然吃了不小的亏！\n"; else
                if( damage < 600 ) return "结果只听「砰」地一声，$n疼得连腰都弯了下来！\n"; else
                if( damage < 800 ) return "结果这一下「轰」地一声砸得$n眼冒金星，差一点摔倒！\n"; else
                if( damage < 1000 )return "结果重重地砸中，$n眼前一黑，「哇」地一声吐出一口鲜血！\n"; else
                                   return "结果只听见「轰」地一声巨响，$n被砸得血肉模糊，惨不忍睹！\n";
                break;
        case "咬伤":
                if( damage < 100 ) return "结果只是轻轻地蹭了一下$p的皮肉。\n";else
                if( damage < 200 ) return "结果在$p$l咬出一排牙印。\n";else
                if( damage < 400 ) return "结果$p被咬下一片肉来！\n";else
                if( damage < 800 ) return "结果$p连皮带肉被咬下一大块！\n";else
                if( damage < 1000 )return "结果在$p的身上咬下来血肉模糊的一大块$l肉！\n";else
                                   return "结果只听见$n一声惨嚎，$p的$l上的肉被一口咬掉，露出骨头！！\n";
                break;
        case "灼伤":
        case "烧伤":
                if( damage < 100 ) return "结果只是把$p的$l烫了一下。\n";else
                if( damage < 200 ) return "结果$p的$l被$w灼炙得起了个小疱。\n";else
                if( damage < 400 ) return "结果$p的$l被$w烧得红肿疼痛，痛得不住冒汗咬牙！\n";else
                if( damage < 800 ) return "结果「嗤」一声轻响，$p祗觉$l上一片热痛，已被$w烧成了暗红色，犹如焦炭！\n";else
                if( damage < 1000 )return "结果「嗤嗤」一阵声响过去，$n神色痛楚难当，$l被$w烧得一片焦黑，青烟直冒！\n";else
                                   return "结果只听见$n长声惨呼，在地上不住打滚，$l已被$w烧得皮肉尽烂，焦臭四溢！！\n";
                break;
        case "冻伤":
                if( damage < 100 ) return "结果$p祗是觉得$l有些冷飕飕地，还挺凉快的。\n";else
                if( damage < 200 ) return "结果$p的$l被冻得有些麻木。\n";else
                if( damage < 400 ) return "结果$p机伶伶地打了个寒颤！\n";else
                if( damage < 800 ) return "结果$p脸色一变，神情有些僵硬，身子也冷得瑟瑟发抖！\n";else
                if( damage < 1000 )return "结果$p身子打颤，脸色苍白，嘴唇冻得发紫，牙关格格直响！\n";else
                                   return "结果祗听见$n声音一哑，脸上惨白得没半分血色，颤抖不休，浑身血液似乎都结成了冰！！\n";
                break;
        default:
                if( !type) type = "伤害";
                if( damage < 50 )  str = "结果只是勉强造成一处轻微"; else
                if( damage < 100 ) str = "结果造成轻微的"; else
                if( damage < 200 ) str = "结果造成一处"; else
                if( damage < 400 ) str = "造成一处严重"; else
                if( damage < 500 ) str = "结果造成颇为严重的"; else
                if( damage < 600 ) str = "结果造成相当严重的"; else
                if( damage < 800 ) str = "结果造成十分严重的"; else
                if( damage < 1000 )str = "结果造成极其严重的"; else
                                   str = "结果造成非常可怕的严重";
                return str + type + "！\n";
        }
}

varargs string eff_status_msg(int ratio, int dam_type) 
{
        if( !dam_type ) dam_type = 1;
        if( ratio==100 ) return HIG "看来身上并无丝毫伤痕。" NOR;
        if( ratio > 95 ) return HIG "似乎只受了点轻伤，但无伤大体。" NOR;
        if( ratio > 90 ) {
                if (dam_type == 3)
                        return HIY "看来有些萎靡，好像精力受损。" NOR;
                else if (dam_type == 2)
                        return HIY "精神似乎受了些损伤，表情有点僵硬。" NOR;
                else    return HIY "可能受了几处皮肉伤。" NOR;
        }
        if( ratio > 80 ) return HIY "受了几处创伤，似乎并不碍事。" NOR;
        if( ratio > 60 ) {
                if (dam_type == 3)
                        return HIY "骨软筋麻，涕泪交流。" NOR;
                else if (dam_type == 2)
                        return HIY "神情惨然，耳鼻中竟然有鲜血渗出。" NOR;
                else    return HIY "身上受伤不轻，正在流着血。" NOR;
        }
        if( ratio > 40 ) {
                if (dam_type == 3)
                        return HIY "面部抽搐不止，已经不能自己。" NOR;
                else    return HIR "流血不止，动作散乱，已经在走下坡路了。" NOR;
        }
        if( ratio > 30 ) return HIR "伤痕累累，正在勉力支撑著不倒下去。" NOR;
        if( ratio > 20 ) return HIR "遍体鳞伤，只剩下最后一口气了。" NOR;
        if( ratio > 10 ) return RED "浑身浴血，就要倒在地上。" NOR;
        if( ratio > 5  ) return RED "瞳孔放大，已经奄奄一息，命在旦夕了。" NOR;
        return RED "如风中残烛，随时都可能断气。" NOR;
}

varargs string status_msg(int ratio, int dam_type) 
{
        if( !dam_type ) dam_type = 1;
        if( ratio==100 ) return HIG "精力充沛，神完气足，" NOR;
        if( ratio > 95 ) return HIG "似乎有些疲惫，但是仍然十分有活力，" NOR;
        if( ratio > 90 ) return HIY "看起来可能有些不济，" NOR;
        if( ratio > 80 ) {
                if (dam_type == 3)
                        return HIY "精力有点不足，但仍攻守从容，" NOR;
                else if (dam_type == 2)
                        return HIY "神情有些恍惚，但仍应付自如，" NOR;
                else    return HIY "动作有点不太灵光，但仍有条不紊，" NOR;
        }
        if( ratio > 60 ) {
                if (dam_type == 3)
                        return HIY "行动虽然无碍，精力明显跟不上了，" NOR;
                else if (dam_type == 2)
                        return HIY "反应开始迟缓，双目无神，" NOR;
                else    return HIY "气喘嘘嘘，脸色惨白，" NOR;
        }
        if( ratio > 40 ) {
                if (dam_type == 3)
                        return HIR "精力涣散，似乎对痛痒都失去知觉，" NOR;
                else if (dam_type == 2)
                        return HIR "眼光迷离，六神无主，" NOR;
                else    return HIR "似乎十分疲惫，看来需要好好休息了，" NOR;
        }
        if( ratio > 30 ) return HIR "头重脚轻，正在勉力支撑著不倒下去，" NOR;
        if( ratio > 20 ) {
                if (dam_type == 3)
                        return HIR "看来已经麻木不仁了，" NOR;
                else if (dam_type == 2)
                        return HIR "看起来已经魂不守舍了，" NOR;
                else    return HIR "看起来已经力不从心了，" NOR;
        }
        if( ratio > 10 ) return RED "摇头晃脑、歪歪斜斜，濒于晕倒，" NOR;
        return RED "已经陷入半昏迷状态，神智全失，" NOR;
}

varargs mixed report_status(object ob, int effective)
{
        int eff_ratio = 100;
        int ratio = 100;
        int k_stat, g_stat, s_stat;
        int d_type = 1;
        int e_type = 1;
        string *msg = ({RED,HIR,HIY,HIG,HIG});
        string str;
 
        if( !ob || !query("max_qi", ob) || !query("max_jing", ob) ) return;
               
        k_stat = (int) query("qi", ob) * 100 / (int)query("max_qi", ob);
        //s_stat = (int) query("sen", ob) * 100 / (int)query("max_sen", ob);
        g_stat = (int) query("jing", ob) * 100 / (int)query("max_jing", ob);
               
        if (ratio > k_stat ) {
                ratio =  k_stat;
        }
        /*
        if (ratio > s_stat ) {
                ratio =  s_stat;
                d_type = 2;
        }
        */
        if (ratio > g_stat ) {
                ratio =  g_stat;
                d_type = 3;
        }
        
        k_stat = (int) query("eff_qi", ob) * 100 / (int)query("max_qi", ob);
        //s_stat = (int) query("eff_sen", ob) * 100 / (int)query("max_sen", ob);
        g_stat = (int) query("eff_jing", ob) * 100 / (int)query("max_jing", ob);
        
        if (eff_ratio > k_stat ) {
                eff_ratio =  k_stat;
        }
        /*
        if (eff_ratio > s_stat ) {
                eff_ratio =  s_stat;
                e_type = 2;
        }
        */
        if (eff_ratio > g_stat ) {
                eff_ratio =  g_stat;
                e_type = 3;
        }
        
        str = "( $N" + status_msg(ratio, d_type) + eff_status_msg(eff_ratio, e_type) + " )\n";
        if( !effective )
                message_vision(str, ob);
        else
                return str;
}

#ifdef REDUCE_PVP_DAMAGE
int reduce_damage(object attacker, object victim, int damage)
{
        if( objectp(attacker) && objectp(victim) &&
            playerp(attacker) && playerp(victim) ) {
                damage = damage * PVP_DAMAGE_SCALE / 100;
        }

        return damage;
}

int reduce_wound(object attacker, object victim, int wound)
{
        if( objectp(attacker) && objectp(victim) &&
            playerp(attacker) && playerp(victim) ) {
                wound = wound * PVP_WOUND_SCALE / 100;
        }

        return wound;
}
#endif

int valid_power(object ob, int combat_exp)
{
        int t;
        
        t = query("reborn/times", ob);

        if( t >= 1 )
                combat_exp += 5000000000/2;
        
        if( t >= 2 )
                combat_exp += 10000000000/2;
        
        if( t >= 3 )
                combat_exp += 20000000000/2;
                
        return combat_exp/100;
}

// This function calculates the combined skill/combat_exp power of a certain
// skill. This value is used for A/(A+B) probability use.
varargs mixed skill_power(object ob, string skill, int usage, int delta)
{
        int level, power;
        mapping dbase;
        mapping temp;
        mapping fight;
        mapping apply;
        int percent = 0;
        int bonus,buff;
        int ratio = 0;
        string martial_skill = ob->query_skill_mapped(skill);

        if( !living(ob) ) return 0; // 晕倒时 power 永远等于零
        if( ob->over_encumbranced() )    return 0; // 负重小于零时 power 永远等于零
        
        if( userp(ob) && ob->is_ghost() )
                return 0;  // 鬼魂的 power 永远等于零
                
        level = ob->query_skill(skill);

        dbase = ob->query_entire_dbase();
        temp = ob->query_entire_temp_dbase();
        if( temp ) 
        {
                apply = temp["apply"];
                fight = temp["fight"];
        }

        switch( usage ) {
        case SKILL_USAGE_ATTACK:
                if( apply )  {
                        level += apply[skill];
                        level += apply["attack"];
                }
                if( query("jingmai/finish", ob) )
                        buff = ZHOUTIAN_D->query_jingmai_effect("attack"); // 经脉冲穴效果 1640
                else
                        buff = query("jingmai/attack", ob);     
                bonus = query_temp("bonus/attack", ob);         // This bonus is ONE_SHOT_BONUS.
                ob->set_temp("bonus/attack", 0);                // after this calculation, release bonus.s
                break;
        case SKILL_USAGE_DEFENSE:
                if( apply ) {
                        level += apply[skill];
                        level += apply["defense"];
                }
                if( query("jingmai/finish", ob) )
                        buff = ZHOUTIAN_D->query_jingmai_effect("defense"); // 经脉冲穴效果 1140
                else
                        buff = query("jingmai/defense", ob);     
                bonus = query_temp("bonus/defense", ob);        // This bonus is ONE_SHOT_BONUS.
                ob->set_temp("bonus/defense", 0);               // after this calculation, release bonus.s
                break;
        case SKILL_USAGE_ABSORB:
                level += apply["iron-cloth"];
                bonus = query_temp("bonus/iron-cloth", ob);        // This bonus is ONE_SHOT_BONUS.
                ob->set_temp("bonus/iron-cloth", 0);               // after this calculation, release bonus.s
                if( !level )
                        return 0;
                break;
        }
        if( delta ) level += delta;

        if( !level || level < 1 ) 
        {
                power = valid_power(ob, query("combat_exp", ob)) / 2;

                if( ob->is_weak() ) power /= 2; // 虚弱 power 减半
                return  power;
        }

        power = level * level * level / 10;
        power+= bonus * bonus * bonus / 10;
        power+= buff * buff * buff / 10;

        if( usage == SKILL_USAGE_ATTACK ) 
        {
                power = power / 30 * dbase["str"];
                if( fight && ob->is_fighting() )
                         power += power / 100 * fight["attack"];
        } 
        else 
        {
                power = power / 30 * dbase["dex"];
                if( fight && ob->is_fighting() )
                         power += power / 100 * fight[skill];
        }
        
        power += valid_power(ob, dbase["combat_exp"]);
        
        
        // 逍遥游提高命中及回避
        if( ob->query_skill("xiaoyao-you", 1) >= 100 )
        {
                percent = ob->query_skill("xiaoyao-you", 1) / 100; // 每100级提升一个百分点
                if( ratio < percent ) ratio = percent;
        }

        // 昆仑无极提高命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("kunlun-wuji", 1) >= 50 && 
            skill == "sword" && martial_skill == "qiankun-jian" )
        {
                percent = ob->query_skill("kunlun-wuji", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 奇门玄术提高命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("qimen-xuanshu", 1) >= 50 &&
            skill == "finger" && martial_skill == "tanzhi-shentong" )
        {
                percent = ob->query_skill("qimen-xuanshu", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 血影大法提高命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("xueying-dafa", 1) >= 50 && 
            skill == "blade" && martial_skill == "xuedao-daofa" )
        {
                percent = ob->query_skill("xueying-dafa", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 静心提高命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("jingxin", 1) >= 50 && 
            ((skill == "sword" && martial_skill == "huifeng-jian")
                || (skill == "strike" && martial_skill == "piaoxue-zhang")) )
        {
                percent = ob->query_skill("jingxin", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 剑意提高命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("jianyi", 1) >= 50 &&
            skill == "sword" && (martial_skill == "huashan-sword" || martial_skill == "lonely-sword") )
        {
                percent = ob->query_skill("jianyi", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 剑意提高防御
        if( usage == SKILL_USAGE_DEFENSE && ob->query_skill("jianyi", 1) >= 50 )
        {
                percent = ob->query_skill("jianyi", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        // 天威正气命中
        if( usage == SKILL_USAGE_ATTACK && ob->query_skill("tianwei-zhengqi", 1) >= 50 &&
            ((skill == "sword" && martial_skill == "taiji-jian")
                || (skill == "cuff" && martial_skill == "taiji-quan")) )
        {
                percent = ob->query_skill("tianwei-zhengqi", 1) / 50;
                if( ratio < percent ) ratio = percent;
        }

        power += power * ratio / 100;      
        
        // 阵法防御效果
        if( (usage == SKILL_USAGE_DEFENSE) && ob->in_array() ) 
        {
                int n;
        
                if( (n =  ob->query_array_level()/100) )
                {
                        power += power * n / 100;
                }
        }

        // 移形换位提升战斗回避率15%
        if( usage == SKILL_USAGE_DEFENSE && query("special_skill/yixing", ob) )
                power += power * 15 / 100;

        // 逆转乾坤提升10%命中
        if( usage == SKILL_USAGE_ATTACK && query("special_skill/nizhuan", ob) )
                power += power * 10 / 100;

        // 流星慧眼提升30%命中
        if( usage == SKILL_USAGE_ATTACK && query("special_skill/huiyan", ob) )
                power += power * 30 / 100;

        // 虚弱状况减半
        if( ob->is_weak() ) power /= 2;
        
        // 手部受伤，使用unarmed，减半
        if( (skill == "unarmed") && (ob->query_limb_wound("右手") || 
            ob->query_limb_wound("左手")) ) 
                power /= 2;

        return power;
}

varargs mapping query_action(object me, int flag)
{
        mixed act,l_act;
        
        //me->reset_action();
        act = query("actions", me); // 此人的右手 action
        if( functionp(act) ) act = evaluate(act, me);
        if( !mapp(act) )
        {
                me->reset_action();
                act = query("actions", me);
                if( functionp(act) ) act = evaluate(act, me);
                if( !mapp(act) ) 
                {
                        CHANNEL_D->do_channel( this_object(), "sys",
                                        sprintf("%s(%s): bad action = %O",
                                        me->name(1), me->query_id(1), 
                                        query("actions", me)));
                        return 0;
                }
        }
        
        if( flag && flag == 1 ) // 指定右手
        {
                set_temp("hand_flag", 0, me);
                return act;
        }
                
        l_act = query("second_actions", me); // 此人的左手 action 
        if( functionp(l_act) ) l_act = evaluate(l_act, me);
        if( !mapp(l_act) || !sizeof(l_act) )
        {
                set_temp("hand_flag", 0, me);
                return act;
        }

        if( flag && flag == 2 ) // 指定左手
        {
                set_temp("hand_flag", 1, me);
                return l_act;
        }

        // 什么因素决定此人用左手攻击还是用右手攻击? 目前没有,暂用随机数决定.
        if( random(100) < 55 ) // 右手的可能性大一些.
        {
                set_temp("hand_flag", 0, me);
                return act;
        }
        else
        {
                set_temp("hand_flag", 1, me);
                return l_act;
        }
}

// do_attack()
//
// Perform an attack action. This function is called by fight() or as an
// interface for some special utilize in quests.
//
varargs int do_attack(object me, object victim, object weapon, int attack_type, int flag)
{
        mapping my, your;
        mapping my_temp, your_temp;
        mapping fight;
        mapping prepare, action;
        string limb, *limbs;
        string attack_skill, martial_skill, force_skill, defense_skill;
        string parry_msg;
        mixed foo;

        int has_weapon;
        int delta;
        int ap, dp, pp;
        int damage, damage_bonus;
        int armor;
        int wounded;
        int array_flag;
        int attack;
        int avoid;

        object second_weapon;
        object weapon2; // weapon of victim
        object ob;

        string result;
        string damage_info;
        string *att;
        int hjs;

        if( !me || !victim ) return 1;
        if( !living(me) ) return 1;
        if( me->is_ghost() || victim->is_ghost() ) return 1;
        
        // 葵花魔功 分身化影
        if( query_temp("hiding_kuihua", victim) ) return 1;
        if( query("no_fight", environment(me))){
                message_vision("$N和$n各自退了一步，收住了招。\n",
                               me, victim);
                me->remove_enemy(victim);
                victim->remove_enemy(me);
                return 0;
        }

        if( environment(me) != environment(victim) ) {
                me->remove_enemy(victim);
                victim->remove_enemy(me);
                return 0;
        }

        hjs = me->query_skill("huajia-su", 1);
        my = copy(me->query_entire_dbase());
        your = copy(victim->query_entire_dbase());
        if( !(my_temp = copy(me->query_entire_temp_dbase())) )
                my_temp = allocate_mapping(5);
        if( !(your_temp = copy(victim->query_entire_temp_dbase())) )
                your_temp = allocate_mapping(5);
        //
        // (0) Choose skills.
        //
        prepare = me->query_skill_prepare();
        if( !prepare ) prepare = ([]);
        
        if( !objectp(weapon) ) weapon = query_temp("weapon", me);
        second_weapon = query_temp("secondary_weapon", me);
        
        if( objectp(weapon))attack_skill=query("skill_type", weapon);
        else if(  sizeof(prepare) == 0) attack_skill = "unarmed";
        else if(  sizeof(prepare) == 1) attack_skill = (keys(prepare))[0];
        else if(  sizeof(prepare) == 2) attack_skill = (keys(prepare))[query_temp("action_flag", me)];

        if( attack_skill == "pin" )
                attack_skill = "sword";

        //
        // (1) Find out what action the offenser will take.
        //
        action = query_action(me, flag);
        if( !action ) return 0;

        if( query_temp("hand_flag", me) )  // 调整'当前兵器' weapon
                weapon = second_weapon;

        has_weapon = objectp(weapon) || objectp(query_temp("armor/hands", me));
        
        if( stringp(query_temp("action_msg", me))){
                result="\n"+query_temp("action_msg", me)+action["action"]+"！\n";
                delete_temp("action_msg", me);
        }else if( query_temp("action_flag", me) == 0){
                result = "\n" + action["action"] + "！\n";
        } else
                result = WHT "\n$N攻势凌厉无匹，瞬间又已攻出一招。\n" NOR + action["action"] + "！\n";
                //result = "\n紧跟着" + action["action"] + "！\n";
        //
        // (2) Prepare AP, DP for checking if hit.
        //
        if( martial_skill = me->query_skill_mapped(attack_skill) )
                limb = SKILL_D(martial_skill)->query_attack_limb(me, victim);

        if( !limb ) {
                limbs=query("limbs", victim);
                if( !arrayp(limbs) ) {
                        limbs = ({ "身体" });
                        set("limbs", limbs, victim);
                }
                limb = limbs[random(sizeof(limbs))];
        }

        // 阵法防御
        if( victim->in_array() && victim->query_array_status() &&
            (random(100) < (victim->query_array_level() / 100 + 60))) {
                if( ((your["qi"] < your["max_qi"] * 2 / 5) ||
                    (your["jing"] < your["max_jing"] * 2 / 5)) &&
                    !ob->is_busy() && objectp(ob = victim->query_array_obstructor()) ) {
                        result = replace_string(result, "$l", limb);
                        if( objectp(weapon) )
                                result = replace_string(result, "$w", weapon->name());
                        else if( stringp(action["weapon"]) )
                                result = replace_string(result, "$w", action["weapon"]);
                        else if( attack_skill == "finger" || attack_skill == "hand" )
                                result = replace_string(result, "$w", "手指" );
                        else if( attack_skill == "strike" || attack_skill == "claw" )
                                result = replace_string(result, "$w", "手掌" );
                        else    result = replace_string(result, "$w", "拳头" );

                        message_combatd(result, me, victim);
                        message_combatd(HIC BWHT "$N飞身移步，挡在了$n的身前。\n" NOR, ob, victim);
                        result = "";

                        victim = ob;
                        your = copy(ob->query_entire_dbase());
                        array_flag = 1;
                        attack_type = TYPE_QUICK;
                }
        }

        fight = ([]);
#ifdef USE_SKILLS_COMBAT
        if( att = SKILLS_D->skill_query_power(martial_skill) ) {
                fight["attack"] = to_int(att[ATTACK]);
                my_temp["fight"] = copy(fight);         // 该技能增加本人命中能力
                fight = ([]);
                fight["dodge"]  = to_int(att[DODGE]);
                fight["parry"]  = to_int(att[PARRY]);
                your_temp["fight"] = copy(fight);       // 该技能降低对手躲闪能力
        } else {
#endif
                if( intp(action["attack"]) )
                        fight["attack"] = action["attack"];
                my_temp["fight"] = copy(fight);
                fight = ([]);
                if( intp(action["dodge"]) )
                        fight["dodge"] = action["dodge"];
                if( intp(action["parry"]) )
                        fight["parry"] = action["dodge"];
                your_temp["fight"] = copy(fight);
#ifdef USE_SKILLS_COMBAT
        }
#endif

        ap = skill_power(me, attack_skill, SKILL_USAGE_ATTACK, 0);
        if( ap < 1) ap = 1;

        if( sizeof(me->query_enemy()) > 1 )
                ap = ap * 9 / (8 + sizeof(victim->query_enemy()));
        if( !me->visible(victim) ) ap /= 10;
        if( member_array(limb, danger_limbs) != -1 )
                ap -= ap / 10;
        if( my["character"] == "阴险奸诈" )
                ap += ap * 20 / 100;

        if( victim->in_array() ) ap /= 2; // 阵法降低命中

        if( array_flag ) dp = 0;          // 替人挡招只能招架
        else {
                dp = skill_power(victim, "dodge", SKILL_USAGE_DEFENSE, 0);
                if( dp < 1 ) dp = 1;

                if( stringp(defense_skill = victim->query_skill_mapped("dodge")) )
                        dp += dp / 100 * SKILL_D(defense_skill)->query_effect_dodge(me, victim);
                if( sizeof(victim->query_enemy()) > 1 )
                        dp = dp * 9 / (8 + sizeof(victim->query_enemy()));
                if( !victim->visible(me) ) dp /= 10;
                if( victim->is_busy() ) dp /= 3;
        }

        //
        // (3) Fight!
        //     Give us a chance of AP/(AP+DP) to "hit" our opponent. Since both
        //     AP and DP are greater than zero, so we always have chance to hit
        //     or be hit.
        //
        damage = 0;
        wounded = 0;
        damage_info = "";

        if( random(ap + dp) < dp && living(victim) ) {
                // Does the victim dodge this hit?
#if INSTALL_COMBAT_TEST
                if( wizardp(me) && query("env/combat_test", me) )
                        tell_object(me, HIY "【战斗精灵】：己方 AP：" + ap +
                                    "，DP：" + dp + "。\n" NOR);
                if( wizardp(victim) && query("env/combat_test", victim) )
                        tell_object(victim, HIC "【战斗精灵】：对方 AP：" + ap +
                                    "，DP：" + dp + "。\n" NOR);
#endif
                if( !defense_skill ) defense_skill = "dodge";
                result += SKILL_D(defense_skill)->query_dodge_msg(limb, victim);

                damage = RESULT_DODGE;
        } else {
        //
        //  (4) Check if the victim can parry this attack.
        //
                delta = 0;
                if( weapon2 = your_temp["weapon"] ) {
                        if( !weapon ) delta = 100;
                } else {
                        if( weapon ) delta = -100;
                }

                pp = skill_power(victim, "parry", SKILL_USAGE_DEFENSE, delta);
                if( stringp(defense_skill = victim->query_skill_mapped("parry")))
                        pp += pp / 100 * SKILL_D(defense_skill)->query_effect_parry(me, victim);
                if( pp < 1 ) pp = 1;
                if( sizeof(victim->query_enemy()) > 1 )
                        pp = pp * 9 / (8 + sizeof(victim->query_enemy()));
                if( victim->is_busy() ) pp /= 3;

                if( random(ap + pp) < pp ) {
#if INSTALL_COMBAT_TEST
                        if( wizardp(me) && query("env/combat_test", me) )
                                tell_object(me, HIY "【战斗精灵】：己方 AP：" + ap +
                                            "，PP：" + pp + "。\n" NOR);
                        if( wizardp(victim) && query("env/combat_test", victim) )
                                tell_object(victim, HIC "【战斗精灵】：对方 AP：" + ap +
                                            "，PP：" + pp + "。\n" NOR);
#endif
                        if( !defense_skill) defense_skill = "parry";
                        if( attack_skill == "throwing" && your["race"] ==  "人类" ) {
                                if( weapon2 )
                                        parry_msg = weapon_parry_throwing[random(sizeof(weapon_parry_throwing))];
                                else {
                                        parry_msg = unarm_parry_throwing[random(sizeof(unarm_parry_throwing))];
                                }
                        } else if( martial_skill == "six-finger" ) {
                                if( random(me->query_skill("six-finger", 1)) > victim->query_skill("force", 1)/2 ) {
                                        if( weapon2 ) {
                                                parry_msg = weapon_parry_sixmai[random(sizeof(weapon_parry_sixmai))];
                                                if( my["jiali"] > weapon2->weight() ) {
                                                        parry_msg = replace_string(parry_msg, "$v", weapon2->name());
                                                        weapon2->unequip();
                                                        weapon2->move(environment(victim));
                                                        set("consistence", 0, weapon2);
                                                        if( weapon2->is_item_make() ) {
                                                                parry_msg += HIW "$n" HIW "只觉得虎口剧震，兵器脱手而出！。\n" NOR;
                                                        } else {
                                                                parry_msg += HIW "只听见「啪」地一声，$n" HIW "手中的" + weapon2->name() +
                                                                             HIW "已被$N" HIW "的无形剑气震为数截！\n" NOR;
                                                                set("name", "断掉的"+query("name", weapon2), weapon2);
                                                                set("value", 0, weapon2);
                                                                set("weapon_prop", 0, weapon2);
                                                        }
                                                        victim->reset_action();
                                                }
                                        } else parry_msg = unarm_parry_sixmai[random(sizeof(unarm_parry_sixmai))];
                                } else  parry_msg = SKILL_D(defense_skill)->query_parry_msg(weapon2, victim);

                        } else if( defense_skill == "douzhuan-xingyi" || defense_skill == "qiankun-danuoyi" ) {
                                if( random(victim->query_skill(defense_skill, 1)) > me->query_skill("force", 1)/2 ) {
                                        if( weapon) {
                                                if( defense_skill == "douzhuan-xingyi" )
                                                        parry_msg = star_move_weapon[random(sizeof(star_move_weapon))];
                                                else
                                                        parry_msg = qiankun_move_weapon[random(sizeof(qiankun_move_weapon))];

                                                damage=query_temp("apply/damage", me);
                                        } else {
                                                if( defense_skill == "douzhuan-xingyi" )
                                                        parry_msg = star_move_unarm[random(sizeof(star_move_unarm))];
                                                else
                                                        parry_msg = qiankun_move_unarm[random(sizeof(qiankun_move_unarm))];

                                                damage=query_temp("apply/unarmed_damage", me);
                                        }

                                        damage+=query_temp("apply/damage1", me);   // 装备附加伤害
                                        damage+=query_temp("apply/damage2", me);   // 装备改造伤害
                                        if( query("jingmai/finish", me) )
                                                damage+= ZHOUTIAN_D->query_jingmai_effect("damage");
                                        damage+=query("jingmai/damage", me);       // 经脉附加伤害

                                        damage += (int)me->query_skill(attack_skill, 1);
                                        damage+=query("jiali", me);
                                        damage = (damage + random(damage)) / 2;
#ifdef USE_SKILLS_COMBAT
                                        if( arrayp(att) && sizeof(att) )
                                                damage += (int)att[DAMAGE] * damage / 100;
                                        else
#endif
                                        if( action["damage"] )
                                                damage += action["damage"] * damage / 100;
                                        
                                        damage_bonus = me->query_str();
#ifdef USE_SKILLS_COMBAT
                                        if( arrayp(att) && sizeof(att) )
                                                damage_bonus += (int)att[FORCE2] * damage_bonus / 100;
                                        else
#endif
                                        if( action["force"] )
                                                damage_bonus += action["force"] * damage_bonus / 100;
                                        // calculate the damage
                                        if( damage_bonus > 0 )
                                                damage += (damage_bonus + random(damage_bonus)) / 2;

                                        me->receive_damage("qi", damage, victim);
                                        armor=query_temp("apply/armor", me);
                                        armor+=query_temp("apply/armor1", me);
                                        armor+=query_temp("apply/armor2", me);
                                        if( query("jingmai/finish", me) )
                                                armor+= ZHOUTIAN_D->query_jingmai_effect("armor");
                                        armor+=query("jingmai/armor", me);
                                        if( damage > armor + 2 )
                                                me->receive_wound("qi", (damage - armor)/2, victim);

                                        foo = damage_msg(damage, action["damage_type"]);
                                        foo = replace_string(foo, "$n", "$N");
                                        foo = replace_string(foo, "$p", "$P");
                                        
                                        /*
                                        foo += "( $N" +
                                                status_msg(query("qi", me)*100/
                                                (query("max_qi", me)?query("max_qi", me):100))+")\n";
                                        */
                                        foo += report_status(me, 1);
                                        parry_msg += foo;
                                } else
                                        parry_msg = SKILL_D(defense_skill)->query_parry_msg(weapon2, victim);
                        } else {
                                // change to SKILL_D(defense_skill) after added parry msg to those
                                // martial arts that can parry.
                                parry_msg = SKILL_D(defense_skill)->query_parry_msg(weapon2, victim);
                        }

                        if( !stringp(parry_msg) )
                                parry_msg = SKILL_D("parry")->query_parry_msg(weapon2, victim);

                        if( stringp(parry_msg) )
                                result += parry_msg;

                        damage = RESULT_PARRY;
                } else {
        //
        //  (5) We hit the victim and the victim failed to parry
        //
                        attack = 1;

                        if( weapon )
                                damage=query_temp("apply/damage", me) +
                                       query_temp("apply/damage", me) * query("jingmai/weapon", me) / 100;
                        else
                                damage=query_temp("apply/unarmed_damage", me) +
                                       query_temp("apply/unarmed_damage", me) * query("jingmai/unarmed_damage", me) / 100;

                        damage+=query_temp("apply/damage1", me);
                        damage+=query_temp("apply/damage2", me);
                        damage+=query("jingmai/damage", me);    // 经脉附加伤害

                        damage = damage + me->query_skill(attack_skill, 1);

                        damage+=query("jiali", me);

#ifdef USE_SKILLS_COMBAT
                        if( arrayp(att) && sizeof(att) )
                                damage += (int)att[DAMAGE] * damage / 100;
                        else
#endif
                        if( action["damage"] )
                                damage += action["damage"] * damage / 100;


                        // 先天技能浩然正气对邪气类效果加倍
                        if( query("special/haoran") && victim->query("shen") < 0 )
                                damage *= 2;
                        else
                        // 先天技能九幽魔气对正气类效果加倍
                        if( query("special/jiuyou") && victim->query("shen") > 0 )
                                damage *= 2;

                        // 昆仑无极的影响
                        if( martial_skill == "qiankun-jian" && me->query_skill("kunlun-wuji", 1) >= 100 )
                                damage += me->query_skill("kunlun-wuji", 1);

                        // 血影大法的影响
                        if( martial_skill == "xuedao-daofa" && me->query_skill("xueying-daofa", 1) >= 100 )
                                damage += me->query_skill("xueying-dafa", 1) * 2;

                        // 臂力的影响
                        damage_bonus = me->query_str();
                        if( my["jianu"] ) {
                                // does angry bonus
                                damage_bonus += me->cost_craze(my["jianu"]);
                                if( me->query_craze() > 1000 &&
                                    random(my["jianu"] + 200) > 200 ) {
                                        damage_info += random(2) ? HIR "$N" HIR "大喝一声，双目圆睁，一股凌厉的杀气油然而起！\n" NOR
                                                                 : HIR "$N" HIR "奋不顾身的扑上前来，招招紧逼$n" HIR "，毫不容情。\n" NOR;
                                        me->cost_craze(200 + random(300));
                                        damage_bonus += my["jianu"] * 2;
                                }
                        }

                        // Clear the special message info after damage info
                        foo_before_hit = 0;
                        foo_after_hit = 0;

                        // Let force skill take effect.
                        if( my["jiali"] && (my["neili"] > my["jiali"]) ) {
                                if( force_skill = me->query_skill_mapped("force") ) {
                                        // 镶嵌有天地日月灵珠的护甲加力伤害减半
                                        if( objectp(ob = query_temp("armor/armor", victim)) && ob->query_magic_item() == "天地日月灵珠" )
                                                delta = 2;
                                        else
                                                delta = 1;

                                        foo = SKILL_D(force_skill)->hit_ob(me, victim, damage_bonus,
                                                my["jiali"] / delta);

                                        if( stringp(foo) ) damage_info += foo; else
                                        if( intp(foo) ) damage_bonus += foo; else
                                        if( mapp(foo) ) {
                                                damage_info += foo["msg"];
                                                damage_bonus += foo["damage"];
                                        }
                                }
                        }
#ifdef USE_SKILLS_COMBAT
                        if( arrayp(att) && sizeof(att) )
                                damage_bonus += (int)att[FORCE2] * damage_bonus / 100;
                        else
#endif
                        if( action["force"] )
                                damage_bonus += action["force"] * damage_bonus / 100;

                        // calculate the damage
                        if( damage_bonus > 0 )
                                damage += (damage_bonus + random(damage_bonus)) / 2;

                        // 根据攻击的部位影响伤害
                        if( !undefinedp(limb_damage[limb]) )
                                damage = (damage * limb_damage[limb] / 100);

                        // Let's attack & parry skill, weapon & armor do effect
                        while( damage > 0)
                        {
                                if( my["not_living"] || your["not_living"] )
                                        break;

                                if( !living(me) || !living(victim) ) break;
                                if( damage < 1 ) break;

                                if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "真武宝石" )
                                        damage_info += HIY "$N" HIY "的真武宝石发出一道耀眼的光芒，另所有人无法辨别眼前事物 ……\n" NOR;
                                else {
                                        delta = 0;
                                        if( attack_skill == "unarmed" && me->query_skill_prepared("unarmed") == "xiantian-gong"
                                        ||  martial_skill == "quanzhen-jian" )
                                                delta = 1;

                                        // Let parry skill take its special effort.
                                        avoid=query_temp("apply/avoid_dodge", me);
                                        if( random(100) >= avoid &&
                                            stringp(defense_skill = victim->query_skill_mapped("dodge")) ) {
                                                foo = SKILL_D(defense_skill)->valid_damage(me, victim, damage, weapon);
                                                if( stringp(foo) ) damage_info += foo; else
                                                if( intp(foo) ) {
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo = foo - foo / 100 * (hjs / 50);
                                                        else
                                                                foo = foo - foo / 100 * (hjs / 100);
                                                        damage += foo;
                                                } else
                                                if( mapp(foo) ) {
                                                        damage_info += foo["msg"];
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 50);
                                                        else
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 100);
                                                        damage += foo["damage"];
                                                }
                                                if( damage < 1 ) break;
                                        }

                                        avoid=query_temp("apply/avoid_parry", me);
                                        if( random(100) >= avoid &&
                                            stringp(defense_skill = victim->query_skill_mapped("parry")) ) {
                                                foo = SKILL_D(defense_skill)->valid_damage(me, victim, damage, weapon);
                                                if( stringp(foo) ) damage_info += foo; else
                                                if( intp(foo) ) {
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo = foo - foo / 100 * (hjs / 50);
                                                        else
                                                                foo = foo - foo / 100 * (hjs / 100);
                                                        damage += foo;
                                                } else
                                                if( mapp(foo) ) {
                                                        damage_info += foo["msg"];
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 50);
                                                        else
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 100);
                                                        damage += foo["damage"];
                                                }
                                                if( damage < 1 ) break;
                                        }

                                        // Let armor/cloth take its special effort
                                        if( mapp(your_temp["armor"]) && objectp(ob = your_temp["armor"]["armor"]) ) {
                                                foo = ob->valid_damage(me, victim, damage, weapon);
                                                if( stringp(foo) ) damage_info += foo; else
                                                if( intp(foo) ) {
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo = foo - foo / 100 * (hjs / 50);
                                                        else
                                                                foo = foo - foo / 100 * (hjs / 100);
                                                        damage += foo;
                                                } else
                                                if( mapp(foo) ) {
                                                        damage_info += foo["msg"];
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 50);
                                                        else
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 100);
                                                        damage += foo["damage"];
                                                }
                                        } else if( mapp(your_temp["armor"]) && objectp(ob = your_temp["armor"]["cloth"]) ) {
                                                foo = ob->valid_damage(me, victim, damage, weapon);
                                                if( stringp(foo) ) damage_info += foo; else
                                                if( intp(foo) ) {
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo = foo - foo / 100 * (hjs / 50);
                                                        else
                                                                foo = foo - foo / 100 * (hjs / 100);
                                                        damage += foo;
                                                } else
                                                if( mapp(foo) ) {
                                                        damage_info += foo["msg"];
                                                        // 发挥化甲术的特效
                                                        if( delta )
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 50);
                                                        else
                                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * (hjs / 100);
                                                        damage += foo["damage"];
                                                }
                                        }
                                        if( damage < 1 ) break;
                                        
                                        /*
                                        // 兵器有透甲能力。如透甲锥之类的暗器
                                        if(objectp(weapon) && weapon->through_armor())
                                                return 0;       // 硬功的防护作用。armor_vs_force
                                        absp = skill_power(victim, "iron-cloth", SKILL_USAGE_ABSORB);
                                        if(absp)
                                        {
                                                defense_skill = victim->query_skill_mapped("iron-cloth");
                                                if(!defense_skill) defense_skill = "iron-cloth";

                                                if(!mute_flag)
                                                        result += SKILL_D(defense_skill)->query_absorb_msg();
                                                foo = SKILL_D(defense_skill)->absorb_ob(victim, me, damage, absp);
                                                if(stringp(foo))
                                                {
                                                        if(!mute_flag)
                                                        result += foo;
                                                }
                                                else if(intp(foo)) damage += foo;
                                        }                       
                                        */
                                }
                                // Let attack skill take its special effort.
                                avoid=query_temp("apply/avoid_attack", victim);
                                if( random(100) >= avoid && stringp(martial_skill) ) {
                                        foo = SKILL_D(martial_skill)->hit_ob(me, victim, damage);
                                        if( stringp(foo) ) damage_info += foo; else
                                        if( intp(foo) ) damage += foo; else
                                        if( mapp(foo) ) {
                                                damage_info += foo["msg"];
                                                damage += foo["damage"];
                                        }
                                        if( damage < 1 ) break;
                                }

                                // Let weapon or living have their special damage.
                                if( weapon ) {
                                        foo = weapon->hit_ob(me, victim, damage);
                                        if( stringp(foo) ) damage_info += foo; else
                                        if( intp(foo) ) damage += foo; else
                                        if( mapp(foo) ) {
                                                damage_info += foo["msg"];
                                                damage += foo["damage"];
                                        }
                                } else {
                                        foo = me->hit_ob(me, victim, damage);
                                        if( stringp(foo) ) damage_info += foo; else
                                        if( intp(foo) ) damage += foo; else
                                        if( mapp(foo) ) {
                                                damage_info += foo["msg"];
                                                damage += foo["damage"];
                                        }
                                }

                                if( damage < 1 ) break;

                                if( mapp(my_temp["apply"]) && my_temp["apply"]["double_damage"] > 0 ) {
                                        if( random(100) < my_temp["apply"]["double_damage"] ) {
                                                int damage2, scale;
                                                damage2 = random(damage);
                                                scale = 100 + damage2 * 10 / damage * 10;
                                                damage += damage2;
                                                damage_info += HIY "$N" HIY "福至心灵，有如神助，一击之中竟产生多重伤害，给予$n" HIY "造成「" + scale + "%」的伤害！\n" NOR;
                                        }
                                }

                                // finish
                                break;
                        }
        //
        //  (6) Inflict the damage.
        //

                        if( victim->query_skill_mapped("dodge") == "lingbo-weibu" &&
                            query_temp("immortal", victim) && random(100) < 90 )
                        {
                                damage = 0;
                                damage_info += HIY "$n" HIY "施展凌波微步逆天绝招「神光离合」，一道耀眼的光芒环绕全身，顿时让所有的攻击落空 ……\n" NOR;
                        }

                        if( damage > 0 ) {
                                /*
                                int def_exp;

                                // Let combat exp take effect
                                def_exp = your["combat_exp"];
                                while( random(def_exp) > my["combat_exp"] ) {
                                        damage -= damage / 4;
                                        def_exp /= 2;
                                }
                                */

                                if( my["character"] == "心狠手辣" )
                                        damage += damage * 20 / 100;

                                if( my["breakup"] ) damage += damage * 10 / 100;

                                // 元神
                                if( query("yuanshen/status", me) )
                                        damage += damage * 50 / 100;

                                if( objectp(ob = query_temp("armor/surcoat", victim)) && ob->query_magic_item() == "流行赶月" )
                                        damage -= damage * 5 / 100;

                                // calculate wounded
                                wounded = damage;
                                armor=query_temp("apply/armor", victim);
                                armor+=query_temp("apply/armor1", victim);
                                armor+=query_temp("apply/armor2", victim);
                                if( query("jingmai/finish", victim) )
                                        armor += ZHOUTIAN_D->query_jingmai_effect("armor");
                                armor+=query("jingmai/armor",victim);

                                // 发挥化甲术的特效
                                if( attack_skill == "unarmed" && me->query_skill_prepared("unarmed") == "xiantian-gong"
                                ||  martial_skill == "quanzhen-jian" )
                                        armor -= armor / 100 * hjs / 50;
                                else
                                        armor -= armor / 100 * hjs / 100;

                                //if( mapp(your_temp["apply"]) && your_temp["apply"]["armor"] > 0 )
                                //        wounded -= random(your_temp["apply"]["armor"]);
                                if( random(100) >= query_temp("apply/through_armor", me) )
                                        wounded -= random(armor);

                                // 昆仑无极的穿透作用
                                if( martial_skill == "qiankun-jian" && me->query_skill("kunlun-wuji", 1) >= 125 )
                                        wounded += me->query_skill("kunlun-wuji", 1) / 125 * 100;

                                // 发挥披风--赤炼龙胆的特效
                                if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "赤炼龙胆" )
                                {
                                        damage_info += HIR "\n$N" HIR "的赤炼龙胆宝石发出一道奇异的光芒，一条发光的"
                                                       "的赤龙张牙舞爪般的扑向$n" HIR "。\n" NOR;
                                        damage += damage / 2;
                                }

                                // 发挥披风--孔 雀 石的效果
                                if( objectp(ob = query_temp("armor/surcoat", victim)) && ob->query_magic_item() == "孔雀石" )
                                {
                                        result += HIG + victim->name() + HIG "的孔雀石发出一道奇异的光芒，数道青光将" + victim->name() +
                                                  HIG "笼罩，但见一只发光的孔雀\n跳跃飞舞，却不让任何人靠近。\n" NOR;

                                        // 发挥披风--真武宝石的效果
                                        if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "真武宝石" )
                                                result += HIY "可是" + me->name() + HIY "的真武宝石发出一道亮光，"
                                                          "那发光的孔雀顿时消失不见。\n" NOR;
                                        else
                                        {
                                                message_combatd(result, me, victim);
                                                return 1;
                                        }
                                }

                                // 发挥勋章--碧海残镢的作用
                                if( objectp(ob = query_temp("armor/medal", me)) && ob->query_magic_item() == "碧海残镢" )
                                {
                                        damage_info += HIC "$N" HIC "的碧海残镢青光一闪，「碧海苍穹」呈现，发出震耳般的海啸声 ……\n" NOR;
                                        damage += damage / 2;
                                        if( !victim->is_busy() )
                                                victim->start_busy(3);
                                }

                                else
                                // 发挥勋章--麒麟火的作用
                                if( objectp(ob = query_temp("armor/medal", me)) && ob->query_magic_item() == "麒麟火" )
                                {
                                        damage_info += HIR "$N" HIR "的麒麟火发闪出火红般的光芒，「麒麟圣火」长久不熄，令人难以喘息 ……\n" NOR;
                                        damage += damage / 2;
                                        if( query("neili", victim) >= 300 )
                                        {
                                                addn("neili", -300, victim);
                                                addn("neili", 300, me);
                                        }
                                }
                                /*
                                if( wounded > 400 )
                                        wounded = (wounded - 400) / 4 + 300;
                                else if( wounded > 200 )
                                        wounded = (wounded - 200) / 2 + 200;
                                else if( wounded < 1 )
                                        wounded = 0;

                                // recalculate damage
                                if( damage > 400 )
                                        damage = (damage - 400) / 4 + 300;
                                else if( damage > 200 )
                                        damage = (damage - 200) / 2 + 200;
                                */
                                if( your["character"] == "光明磊落" )
                                        wounded -= wounded * 20 / 100;

#ifdef REDUCE_PVP_DAMAGE
                                damage = COMBAT_D->reduce_damage(me, victim, damage);
#endif
                                damage = victim->receive_damage("qi", damage, me);
                                if( damage < 1 )
                                        damage_info += BLU "$n" BLU "神奇的化解了$N" BLU "这一击的所有伤害！\n" NOR;

                                if( wounded > 0 &&
                                    // ((me->is_killing(your["id"]) && random(3) == 1) ||
                                    (has_weapon ||
                                     random(3) == 1) ) {
                                        // We are sure that damage is greater than victim's armor here.
#ifdef REDUCE_PVP_DAMAGE
                                        wounded = COMBAT_D->reduce_wound(me, victim, wounded);
#endif
                                        wounded = victim->receive_wound("qi", wounded, me);
                                } else  wounded = 0;

                                // add message before hit in victim
                                if( foo_before_hit )
                                        result += foo_before_hit;

                                result += damage_msg(damage, action["damage_type"]);
                                /*
                                damage_info += "( $n" +
                                        status_msg(query("qi", victim)*100/
                                        (query("max_qi", victim)?query("max_qi", victim):100))+")\n";
                                */
                                damage_info += replace_string(report_status(victim, 1), "$N", "$n");
                                // 奇门玄术的反伤效果
                                if( victim->query_skill("qimen-xuanshu", 1) >= 100 )
                                {
                                        if( random(victim->query_skill("qimen-xuanshu", 1)) > 100 && random(3) == 1 )
                                        {
                                                damage_info += HIG "$n" HIG "施展出「奇门玄术」的反伤效果，使得$N" HIG "自身同时也受到损伤。\n" NOR;
                                                me->receive_damage("qi", damage * victim->query_skill("qimen-xuanshu", 1) / 5000, victim);
                                                me->receive_wound("qi", wounded * victim->query_skill("qimen-xuanshu", 1) / 5000, victim);

                                                if( query("eff_qi", victim) + wounded * victim->query_skill("qimen-xuanshu", 1) / 5000 <
                                                    query("max_qi", victim) )
                                                        addn("eff_qi", wounded * victim->query_skill("qimen-xuanshu", 1) / 5000, victim);
                                                else
                                                        set("eff_qi", query("max_qi", victim), victim);

                                                if( query("qi", victim) + damage * victim->query_skill("qimen-xuanshu", 1) / 5000 <
                                                    query("eff_qi", victim) )
                                                        addn("qi", damage * victim->query_skill("qimen-xuanshu", 1) / 5000, victim);
                                                else
                                                       set("qi", query("eff_qi", victim), victim);
                                                
                                                /*
                                                damage_info += "( $N" +
                                                        status_msg(query("qi", me)*100/
                                                        (query("max_qi", me)?query("max_qi", me):100))+")\n";
                                                */
                                                damage_info += report_status(me, 1);
                                        }
                                }

                                // 血影大法伤内力效果
                                if( martial_skill == "xuedao-daofa" && me->query_skill("xueying-dafa", 1) >= 1 )
                                {
                                        addn("neili", -1 * (me->query_skill("xueying-dafa", 1) + me->query_skill("xuedao-daofa", 1)) / 12, victim);
                                        if( query("neili", victim) < 0 )
                                                set("neili", 0, victim);
                                }

                                // 极品防具伤害反射效果
                                avoid=query_temp("apply/counter_damage", victim);
                                if( random(100) < avoid && random(3) == 1 ) {
                                        if( objectp(ob=query_temp("armor/armor", victim)) &&
                                            query("enchase/apply_prop/counter_damage", ob) )
                                                damage_info += BLINK + HIM + ob->name() + BLINK + HIM "发出一道奇异眩目的红光袭向$N！\n" NOR;
                                        else
                                        if( objectp(ob=query_temp("armor/cloth", victim)) &&
                                            query("enchase/apply_prop/counter_damage", ob) )
                                                damage_info += BLINK + HIM + ob->name() + BLINK + HIM "发出一道奇异眩目的红光袭向$N！\n" NOR;
                                        else
                                                damage_info += BLINK + HIM "$n" BLINK + HIM "周身发出一道奇异眩目的红光袭向$N！\n" NOR;

                                        me->receive_damage("qi", damage/5, victim);
                                        me->receive_wound("qi", wounded/5, victim);
                                        /*
                                        damage_info += "( $N" +
                                                status_msg(query("qi", me)*100/
                                                (query("max_qi", me)?query("max_qi", me):100))+")\n";
                                        */
                                        damage_info += report_status(me, 1);
                                }
                        }

                        if( foo_after_hit )
                                damage_info += foo_after_hit;
        //
        //  (7) Give experience
        //
                        /*
                        if( victim->is_boss() && query("fight_boss", victim) &&
                            my["level"] < your["level"]*2 )
                                addn("combat_exp", me->query_int()/10, me);

                        if( your["combat_exp"] > my["combat_exp"] &&
                            (random(my["jing"]*100/my["max_jing"] + me->query_int()) > 50) ) {
                                if( my["combat_exp"] > EXP_LIMIT )
                                        addn("combat_exp", me->query_int()/60, me);
                                else
                                        addn("combat_exp", me->query_int()/15, me);
                                me->improve_skill(attack_skill, random(me->query_int()));
                                if( my["potential"] - my["learned_points"] < 10*me->query_int() )
                                        addn("potential", 1, me);

                        }
                        */
                }
        }

        result = replace_string(result, "$l", limb);
        if( objectp(weapon2=query_temp("weapon", victim)) )
                result = replace_string(result, "$v", weapon2->name());
        if( objectp(weapon) )
                result = replace_string(result, "$w", weapon->name());
        else if( stringp(action["weapon"]) )
                result = replace_string(result, "$w", action["weapon"]);
        else if( attack_skill == "finger" || attack_skill == "hand" )
                result = replace_string(result, "$w", "手指" );
        else if( attack_skill == "strike" || attack_skill == "claw" )
                result = replace_string(result, "$w", "手掌" );
        else    result = replace_string(result, "$w", "拳头" );

        message_combatd(result, me, victim, damage_info);

        if( damage > 0 ) {
#if INSTALL_COMBAT_TEST
                if( userp(me)/* && query("env/combat_test", me)*/ )
                        tell_object(me, HIW "( 你对" +
                                        query("name", victim)+HIW"造成"+
                                        damage + "点伤害，" +
                                        wounded + "点创伤。)\n" NOR);
                if( userp(victim)/* && query("env/combat_test", victim)*/ )
                        tell_object(victim, HIG "( 你受到" +
                                        query("name", me)+HIG+damage+"点伤害，"+
                                        wounded + "点创伤。)\n" NOR);
#endif
                if( attack == 1 ) {
                        switch( limb ) {
                        // 头、颈受伤会伤一些神。
                        case "头部":
                        case "颈部":
                                damage /= 4;
                                if( damage < 1 )
                                        damage = 1;
                                if( damage > 8000 )
                                        damage = 8000;
                                victim->receive_damage("jing", damage, me );
                                delta=query("jing", victim)*100/query("max_jing", victim);
                                if( delta <= 30 )
                                        tell_object(victim, head_damage_me_msg[2]);
                                else if( delta <= 70 )
                                        tell_object(victim, head_damage_me_msg[1]);
                                else
                                        tell_object(victim, head_damage_me_msg[0]);
                                break;
                        // 击中这些部位真气会受到振荡。
                        case "胸口":
                        case "后心":
                        case "小腹":
                                if( query("max_neili", victim) && query("neili", victim)){
                                        damage /= 4;
                                        if( damage < 1 )
                                                damage = 1;
                                        if( damage > 10000 )
                                                damage = 10000;
                                        if( query("neili", victim) <= damage )
                                                set("neili", 0, victim);
                                        else
                                                addn("neili", -damage, victim);
                                        if( damage <= 500 )
                                                foo = body_damage_me_msg[0];
                                        else if( damage <= 1000 )
                                                foo = body_damage_me_msg[1];
                                        else
                                                foo = body_damage_me_msg[2];
                                        foo = replace_string(foo, "$l", limb);
                                        tell_object(victim, foo);
                                }
                                break;
                        }
                        victim->receive_limbs_wound(limb, damage);
                }

                // 显示玩家精气、气血
                /*
                if( query("env/verbose", me) )
                {
                        tell_object(me, SAVEC);
                        tell_object(me, SETDISPLAY(21,49)+DELLINE);
                        tell_object(me, SETDISPLAY(22,49)+DELLINE);
                        tell_object(me, SETDISPLAY(23,49)+DELLINE);
                        tell_object(me, SETDISPLAY(21,50)+HIY"┏━━━━━━━━━━━━━┓");
                        tell_object(me, SETDISPLAY(22,50)+HIY"┃");
                        tell_object(me, SETDISPLAY(22,52)+HIG"精："HIR+query("jing",me)+"/"+query("eff_jing",me));
                        tell_object(me, SETDISPLAY(22,65)+HIG"气："HIR+query("qi",me)+"/"+query("eff_qi",me));
                        tell_object(me, SETDISPLAY(22,78)+HIY"┃");
                        tell_object(me, SETDISPLAY(23,50)+HIY"┗━━━━━━━━━━━━━┛"NOR);
                        tell_object(me, REST);
                }
                */

                if( query("special_skill/monster", victim) && random(5) == 1){
                        message_vision(HIG "$N" HIG "施展出「天妖临世」，$n" HIG "自身同时也受到损伤。\n" NOR, victim, me);
                        me->receive_damage("qi", damage, victim);
                        if( random(2) == 1 ) me->receive_wound("qi", wounded, victim);
                        //damage_info = "(" + me->name() + status_msg(query("qi", me) * 100 / query("max_qi", me)) + ")\n";
                        damage_info = report_status(me, 1);
                        message_vision(damage_info, me);
                }

                if( query("special_skill/demon", victim) && random(5) == 1 )
                        message_vision(HIW "$N" HIW "强提一口丹田之气，真气运转之下，力保心脉不死！\n" NOR, victim, me);
                        victim->receive_curing("qi", victim->query("max_qi") / 3);

                if( victim->is_busy() ) {
                        victim->interrupt_me(me, 30 + random(10));
                        message_vision(HIW "$N行动微觉受制，急忙默运真气，移开了周身要穴！\n" NOR, victim);
                }

                if( victim->query_condition("no_perform") && !random(6) ) {
                        message_vision(HIY"$N发现自己气血运行不畅，急忙默运真气，硬是压下了翻腾不已的气血！\n"NOR, victim);
                        victim->clear_condition("no_perform");
                }

                if( me->query_condition("no_exert") && !random(6) ) {
                        message_vision(HIC"$N发现自己内息不匀，急忙默运真气，调理内息！\n"NOR, victim);
                        victim->clear_condition("no_exert");
                }
                
                if( (!me->is_killing(your["id"])) &&
                    (!victim->is_killing(my["id"])) &&
                    !query("not_living", victim) &&
                    your["qi"] * 2 <= your["max_qi"] ) {
                        me->remove_enemy(victim);
                        victim->remove_enemy(me);
                        if( query("can_speak", me) && query("can_speak", victim) )
                                message_vision(winner_msg[random(sizeof(winner_msg))],
                                               me, victim);
                        if( me == victim->query_competitor() ) {
                                me->win();
                                victim->lost();
                        }
                }
                // if(me->query("special_skill/tianmo") && random(5) == 1) {
                //         message_vision(HIR"$N脑中浮现出天魔解体时所领会的武学精华，以敌血补己气，攻势更显凌厉！\n" NOR, me, victim);
                //         me->receive_curing("qi", (me->query("max_qi")*(int)me->query("special_skill/tianmo",1)*2/100));
                //         do_attack(me, victim, weapon, 3);
                // }
                //
                
        }
        if( functionp(action["post_action"]) )
                evaluate( action["post_action"], me, victim, weapon, damage);
        else
                WEAPON_D->bash_weapon(me, victim, weapon, damage);

        // See if the victim can make a riposte.
        if( attack_type == TYPE_REGULAR &&
            damage < 1 &&
            your_temp["guarding"] ) {
                your_temp["guarding"];
                if( random(my["dex"]) < 5 ) {
                        message_combatd("$N一击不中，露出了破绽！\n",
                                        me, 0, "");
                        do_attack(victim, me, your["weapon"],
                                  TYPE_QUICK);
                } else {
                        message_combatd("$N见$n攻击失误，趁机发动攻击！\n",
                                        victim, me, "");
                        do_attack(victim, me, your_temp["weapon"],
                                  TYPE_RIPOSTE);
                }
        }
}

// do damage when perform
// I will consider the damage of the weapon
// what is final: final may be a function with result type string,
// or a single string.
// The syntax of the final string is info:damage_type@limb or info only,
// If the limb if ?, the system will auto select limb of
// target, if the final is a single string, the system will
// treat as damage infomation.
varargs string do_damage(object me, object target, mixed type,
                         int damage, int percent, mixed final)
{
        mapping my;
        object weapon;
        object cloth;
        object ob;
        int apply;
        int armor;
        int wound;
        int jiali;
        int bonus;
        mixed foo;
        string msg;
        string skill;
        string *limbs;
        string limb;
        string damage_type;
        string result;
        int str;
        int damage_bonus;
        mapping prepare;
        string attack_skill;
        int avoid;
        int def_exp, att_exp;
        int hjs;

        if( damage < 0 ) return "";
        if( me->is_ghost() || target->is_ghost() ) return "";
        if( environment(me) != environment(target) ) {
                me->remove_enemy(target);
                target->remove_enemy(me);
                return "";
        }

        hjs = me->query_skill("huajia-su", 1);

        // Am I use weapon
        if( type == WEAPON_ATTACK ) {
                apply=query_temp("apply/damage", me);
                apply+=query_temp("apply/damage1", me);
                apply+=query_temp("apply/damage2", me);
                apply+=apply*query("jingmai/weapon",me)/100;
                weapon=query_temp("weapon", me);
        } else if( type == UNARMED_ATTACK ) {
                apply=query_temp("apply/unarmed_damage", me);
                apply+=query_temp("apply/damage1", me);
                apply+=query_temp("apply/damage2", me);
                apply+=apply*query("jingmai/unarmed_damage",me)/100;
                weapon = 0;
        } else if( type == REMOTE_ATTACK ) {
                apply  = 0;
                weapon = 0;
        } else {
                if( query_temp("weapon", me)){
                        apply=query_temp("apply/damage", me);
                        apply+=apply*query("jingmai/weapon",me)/100;
                        weapon=query_temp("weapon", me);
                } else {
                        apply=query_temp("apply/unarmed_damage", me);
                        apply+=apply*query("jingmai/unarmed_damage",me)/100;
                        weapon = 0;
                }
                apply+=query_temp("apply/damage1", me);
                apply+=query_temp("apply/damage2", me);
        }

        prepare = me->query_skill_prepare();
        if( !prepare ) prepare = ([]);

        if( type == REMOTE_ATTACK )
                skill = "force";
        else if( weapon )
                skill=query("skill_type", weapon);
        else {
                switch( sizeof(prepare) ) {
                case 0: skill = "unarmed"; break;
                case 1:
                case 2: skill = (keys(prepare))[0]; break;
                }
        }

        if( skill == "pin" ) skill = "sword";

        attack_skill = me->query_skill_mapped(skill);

        // Check the target's armor
        armor=query_temp("apply/armor", target);
        armor+=query_temp("apply/armor1", target);
        armor+=query_temp("apply/armor2", target);
        armor+=query("jingmai/armor", target) * 100;

        damage += apply;
        damage += query("jingmai/damage",me);

        // 发挥昆仑无极的威力
        if( me->query_skill("kunlun-wuji", 1) >= 100 )
                damage += me->query_skill("kunlun-wuji", 1);

        msg = "";
        foo_after_hit = 0;

        while (type == UNARMED_ATTACK || type == WEAPON_ATTACK )
        {
                if( !living(target) ) break;

                if( random(100) < 20 &&
                   (attack_skill == "six-finger" ||
                    attack_skill == "dragon-strike" ||
                    attack_skill == "moon-blade" ||
                    attack_skill == "lonely-sword") )
                        break;

                if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "真武宝石" )
                        msg += HIY "$N" HIY "的真武宝石发出一道耀眼的光芒，另所有人无法辨别眼前事物 ……\n" NOR;
                else
                {
                        avoid=query_temp("apply/avoid_parry", me);
                        skill = target->query_skill_mapped("parry");

                        // Let parry skill take its special effort
                        if( stringp(skill) && (skill == "qiankun-danuoyi" ||
                            skill == "douzhuan-xingyi") ) {
                                foo = SKILL_D(skill)->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) damage += foo; else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        damage += foo["damage"];
                                }
                        } else if( random(100) >= avoid && stringp(skill) ) {
                                foo = SKILL_D(skill)->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) {
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo = foo - foo / 100 * hjs / 100;
                                        damage += foo;
                                } else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * hjs / 100;
                                        damage += foo["damage"];
                                }
                        }

                        if( damage < 1 ) break;

                        avoid=query_temp("apply/avoid_dodge", me);
                        skill = target->query_skill_mapped("dodge");

                        // Let dodge skill take its special effort
                        if( stringp(skill) && skill == "lingbo-weibu") {
                                foo = SKILL_D(skill)->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) damage += foo; else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        damage += foo["damage"];
                                }
                        } else if( random(100) > avoid && stringp(skill) ) {
                                foo = SKILL_D(skill)->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) {
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo = foo - foo / 100 * hjs / 100;
                                        damage += foo;
                                } else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * hjs / 100;
                                        damage += foo["damage"];
                                }
                        }

                        if( damage < 1) break;

                        // Let cloth & armor take its special effort
                        if( cloth=query_temp("armor/armor", target)){
                                foo = cloth->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) {
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo = foo - foo / 100 * hjs / 100;
                                        damage += foo;
                                } else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * hjs / 100;
                                        damage += foo["damage"];
                                }
                        }else if( cloth=query_temp("armor/cloth", target)){
                                foo = cloth->valid_damage(me, target, damage, weapon);
                                if( stringp(foo) ) msg += foo; else
                                if( intp(foo) ) {
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo = foo - foo / 100 * hjs / 100;
                                        damage += foo;
                                } else
                                if( mapp(foo) ) {
                                        msg += foo["msg"];
                                        // 发挥化甲术的特效
                                        if( hjs > 100 )
                                                foo["damage"] = foo["damage"] - foo["damage"] / 100 * hjs / 100;
                                        damage += foo["damage"];
                                }
                        }
                }
                // Finish
                break;
        }

        if( target->query_skill_mapped("dodge") == "lingbo-weibu" &&
            query_temp("immortal", target) && random(100) < 90 )
        {
                damage = 0;
                msg += HIY "$n" HIY "施展凌波微步逆天绝招「神光离合」，一道耀眼的光芒环绕全身，顿时化去所有的伤害 ……\n" NOR;
        }

        /*
        // Let combat exp take effect
        if( damage > 0 )
        {
                def_exp=query("combat_exp", target);
                att_exp=query("combat_exp", me);
                while( random(def_exp) > att_exp ) {
                        damage -= damage / 4;
                        def_exp /= 2;
                }
        }
        */

        while (damage > 0)
        {
                if( target->is_busy() ) target->interrupt_me(target, 30 + random(10));

                // Let weapon/unarmed attack take its special effort
                if( type == WEAPON_ATTACK && objectp(weapon) )
                        // weapon attack
                        foo = weapon->hit_ob(me, target, damage);
                else if( type == UNARMED_ATTACK )
                        // unarmed attack
                        foo = me->hit_ob(me, target, damage);
                else if( type == SPECIAL_ATTACK ) {
                        if( objectp(weapon) )
                                foo = weapon->hit_ob(me, target, damage);
                        else
                                foo = me->hit_ob(me, target, damage);
                } else if( objectp(type) )
                        // special item attack
                        foo = type->hit_ob(me, target, damage);

                if( stringp(foo) ) msg += foo; else
                if( intp(foo) ) damage += foo; else
                if( mapp(foo) ) {
                        msg += foo["msg"];
                        damage += foo["damage"];
                }

                // do enforce effect
                my = me->query_entire_dbase();
                if( (jiali = my["jiali"]) > 0 && my["neili"] >= jiali ) {
                        // eforced
                        my["neili"] -= jiali;

                        if( objectp(ob = query_temp("armor/armor", target)) && ob->query_magic_item() == "天地日月灵珠" )
                                jiali /= 2;

                        if( type == REMOTE_ATTACK || type == SPECIAL_ATTACK ) {
                                damage_bonus = SKILLS_D->enforce_power(me, jiali);
                        } else {
                                damage_bonus = SKILLS_D->enforce_power(me, jiali);

                                if( !living(target) ) break;
                                // check special force effort
                                skill = target->query_skill_mapped("force");
                                if( stringp(skill) && (!objectp(ob = query_temp("armor/surcoat", me)) || ob->query_magic_item() != "真武宝石") ) {
                                        foo = SKILL_D(skill)->valid_damage(me, target, damage_bonus);
                                        if( stringp(foo) ) msg += foo; else
                                        if( intp(foo) ) damage_bonus += foo; else
                                        if( mapp(foo) ) {
                                                msg += foo["msg"];
                                                damage_bonus += foo["damage"];
                                        }
                                }
                        }
                        damage += damage_bonus;
                }

                if( damage < 1 ) break;

                if( query("breakup", me) ) damage += damage * 10 / 100;

                // 元神
                if( query("yuanshen/status", me) )
                        damage += damage * 50 / 100;

                if( objectp(ob = query_temp("armor/surcoat", target)) && ob->query_magic_item() == "流行赶月" )
                        damage -= damage * 5 / 100;

                // 发挥披风--赤炼龙胆的特效
                if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "赤炼龙胆" )
                {
                        msg += HIR "\n$N" HIR "的赤炼龙胆宝石发出一道奇异的光芒，一条发光的"
                               "的赤龙张牙舞爪般的扑向$n" HIR "。\n" NOR;
                        damage += damage / 2;
                }

                // 发挥勋章--碧海残镢的作用
                if( objectp(ob = query_temp("armor/medal", me)) && ob->query_magic_item() == "碧海残镢" )
                {
                        msg += HIC "$N" HIC "的碧海残镢青光一闪，「碧海苍穹」呈现，发出震耳般的海啸声 ……\n" NOR;
                        damage += damage / 2;
                        if( !target->is_busy() )
                                target->start_busy(3);
                }

                else
                // 发挥勋章--麒麟火的作用
                if( objectp(ob = query_temp("armor/medal", me)) && ob->query_magic_item() == "麒麟火" )
                {
                        msg += HIR "$N" HIR "的麒麟火发闪出火红般的光芒，「麒麟圣火」长久不熄，令人难以喘息 ……\n" NOR;
                        damage += damage / 2;
                        if( query("neili", target) >= 300 )
                        {
                                addn("neili", -300, target);
                                addn("neili", 300, me);
                        }
                }

                // 发挥披风--孔 雀 石的效果
                if( objectp(ob = query_temp("armor/surcoat", target)) && ob->query_magic_item() == "孔雀石" )
                {
                        msg += HIG + target->name() + HIG "的孔雀石发出一道奇异的光芒，数道青光将" +
                               target->name() +  HIG "笼罩，但见一只发光的孔\n雀跳跃飞舞，却不让任何人靠近。\n" NOR;

                        // 发挥披风--真武宝石的效果
                        if( objectp(ob = query_temp("armor/surcoat", me)) && ob->query_magic_item() == "真武宝石" )
                        {
                                msg += HIY "可是" + me->name() + HIY "的真武宝石发出一道亮光，那发光"
                                       "的孔雀顿时消失不见。\n" NOR;

                        }
                        else
                                return msg;
                }

                if( query_temp("apply/double_damage", me)>0){
                        if( random(100)<query_temp("apply/double_damage", me)){
                                int damage2, scale;
                                damage2 = random(damage);
                                scale = 100 + damage2 * 10 / damage * 10;
                                damage += damage2;
                                msg += HIY "$N" HIY "福至心灵，有如神助，一击之中竟产生多重伤害，给予$n" HIY "造成「" + scale + "%」的伤害！\n" NOR;
                        }
                }

                // do str effect
                str=me->query_str()+query_temp("str", me)+query("jingmai/str", me);
                if( str < 20 )
                        damage += damage * str / 50;
                else if( str < 40 )
                        damage += damage * ((str - 20) / 2 + 20) / 50;
                else
                        damage += damage / 50 * ((str - 40) / 4 + 30);

                // recalculate damage
                if( damage > 1500 )
                        damage = (damage - 1500) / 4 + 1000;
                else if( damage > 500 )
                        damage = (damage - 500) / 2 + 500;

                // do damage
#ifdef REDUCE_PVP_DAMAGE
                damage = COMBAT_D->reduce_damage(me, target, damage);
#endif
                // 发挥化甲术的特效
                if( hjs > 100 )
                        armor = armor - armor / 100 * hjs / 100;

                if( random(100) >= query_temp("apply/through_armor", me) )
                        wound = (damage - random(armor)) * percent / 100;
                else
                        wound = damage * percent / 100;

                if( me->query_skill("kunlun-wuji", 1) >= 125 )
                        wound += wound + me->query_skill("kunlun-wuji", 1) / 125 * 100;

                // 血影大法伤内力效果
                if( me->query_skill("xueying-dafa", 1) >= 100 )
                {
                        addn("neili", -1 * (me->query_skill("xueying-dafa", 1) + me->query_skill("xuedao-daofa", 1)) / 12, target);
                        if( query("neili", target) < 0 )
                                set("neili", 0, target);
                }

                damage = target->receive_damage("qi", damage, me);
                if( damage < 1 )
                        msg += BLU "$n" BLU "神奇的化解了$N" BLU "这一击的所有伤害！\n" NOR;

                if( wound > 0 ) {
#ifdef REDUCE_PVP_DAMAGE
                        wound = COMBAT_D->reduce_wound(me, target, wound);
#endif
                        wound = target->receive_wound("qi", wound, me);
                }
                else wound = 0;

                if( functionp(final) ) final = evaluate(final);
                if( stringp(final) ) {
                        if( sscanf(final, "%s@%s", final, limb) == 2 ) {
                                if( sscanf(final, "%s:%s", result, damage_type) != 2 ) {
                                        result = HIR "这招打了个正中！";
                                        damage_type = final;
                                }
                                if( sscanf(limb, "?%*s") ) {
                                        // auto select limb
                                        limbs=query("limbs", target);
                                        if( !arrayp(limbs) ) {
                                                limbs = ({ "身体" });
                                                set("limbs", limbs, target);
                                        }
                                        limb = limbs[random(sizeof(limbs))];
                                }

                                // indicate damage type & limb
                                final = replace_string(damage_msg(damage, damage_type), "$l", limb);
                                final = replace_string(final, "$n", "$n" HIR);
                                final = replace_string(final, "$N", "$N" HIR);
                                final = replace_string(final, "$p", "$p" HIR);
                                final = replace_string(final, "$P", "$P" HIR);
                                if( weapon )
                                        final = replace_string(final, "$w", weapon->name() + HIR);
                                final = result + HIR + final + NOR;
                        }
                        msg += final;
                        /*
                        msg +="($n"+status_msg(query("qi", target)*100/(query("max_qi", target)?
                                                   query("max_qi", target):100))+")\n";
                        */
                        msg += replace_string(report_status(target, 1), "$N", "$n");
                }
#if INSTALL_COMBAT_TEST
                msg += HIW "( $N" HIW "对$n" HIW "造成" + damage + "点伤害，" + wound + "点创伤。)\n" NOR;
#endif

                // 奇门玄术的反伤效果
                if( target->query_skill("qimen-xuanshu", 1) >= 100 )
                {
                        if( random(target->query_skill("qimen-xuanshu", 1)) > 100 && random(3) == 1 )
                        {
                                msg += HIG "$n" HIG "施展出「奇门玄术」的反伤效果，使得$N" HIG "自身同时也受到损伤。\n" NOR;
                                // 反伤
                                me->receive_damage("qi", damage * target->query_skill("qimen-xuanshu", 1) / 5000, target);
                                me->receive_wound("qi", wound * target->query_skill("qimen-xuanshu", 1) / 5000, target);

                                if( query("eff_qi", target) + wound * target->query_skill("qimen-xuanshu", 1) / 5000 <
                                    query("max_qi", target) )
                                        addn("eff_qi", wound * target->query_skill("qimen-xuanshu", 1) / 5000, target);
                                else
                                        set("eff_qi", query("max_qi", target), target);

                                if( query("qi", target) + damage * target->query_skill("qimen-xuanshu", 1) / 5000 <
                                    query("eff_qi", target) )
                                        addn("qi", damage * target->query_skill("qimen-xuanshu", 1) / 5000, target);
                                else
                                       set("qi", query("eff_qi", target), target);
                                
                                /*
                                msg += "( $N" +
                                        status_msg(query("qi", me)*100/
                                        (query("max_qi", me)?query("max_qi", me):100))+")\n";
                                */
                                msg += report_status(me, 1);
                        }
                }

                // 极品防具反噬效果
                avoid=query_temp("apply/counter_damage", target);
                if( random(100) < avoid && random(5) == 1 ) {
                        if( objectp(cloth=query_temp("armor/armor", target)) &&
                            query("enchase/apply_prop/counter_damage", cloth) )
                                msg += BLINK + HIM + cloth->name() + BLINK + HIM "发出一道奇异眩目的红光袭向$N！\n" NOR;
                        else
                        if( objectp(cloth=query_temp("armor/cloth", target)) &&
                            query("enchase/apply_prop/counter_damage", cloth) )
                                msg += BLINK + HIM + cloth->name() + BLINK + HIM "发出一道奇异眩目的红光袭向$N！\n" NOR;
                        else
                                msg += BLINK + HIM "$n" BLINK + HIM "周身发出一道奇异眩目的红光袭向$N！\n" NOR;

                        me->receive_damage("qi", damage/5, target);
                        me->receive_wound("qi", wound/5, target);
                        /*
                        msg += "( $N" +
                                status_msg(query("qi", me)*100/
                                (query("max_qi", me)?query("max_qi", me):100))+")\n";
                        */
                        msg += report_status(me, 1);
                }
                if( query("special_skill/monster", target) && random(5) == 1){
                        message_vision(HIG "$N" HIG "施展出「天妖临世」，$n" HIG "自身同时也受到损伤。\n" NOR, target, me);
                        me->receive_damage("qi", damage, target);
                        if( random(2) == 1 ) me->receive_wound("qi", wound, target);
                        /*
                        msg += "( $N" +
                                status_msg(query("qi", me)*100/
                                (query("max_qi", me)?query("max_qi", me):100))+")\n";
                        */
                        msg += report_status(me, 1);
                }
                break;
        }

        if( foo_after_hit ) msg += foo_after_hit;
        if( foo_before_hit ) msg = foo_before_hit + msg;
        // Clear the special message info after damage info
        foo_before_hit = 0;
        foo_after_hit = 0;

        return msg;
}

//  auto_perform()
int auto_perform(object me, object target, string skill)
{
        int l;
        string dir;
        string file;
        mixed *all_file;

        if( !stringp(dir = SKILL_D(skill) + "/") ||
            file_size(dir) != -2 )
                return 0;

        if( file_size(dir + "perform/") == -2 ) {
                all_file = get_dir(dir + "perform/");
                if( !sizeof(all_file) )
                        all_file = get_dir(dir);
                else
                        dir += "perform/";
        } else
                all_file = get_dir(dir);

        if( !sizeof(all_file) )
                return 0;

        all_file = filter_array(all_file, (: is_c_file($1) :));
        if( !sizeof(all_file) )
                return 0;

        file = all_file[random(sizeof(all_file))];
        l = strlen(file);
        file = dir + file[0..l-3];
        return (int)call_other(file, "perform", me, target);
}

//  fight()
//
//  This is called in the attack() defined in F_ATTACK, which handles fighting
//  in the heart_beat() of all livings. Be sure to optimize it carefully.
//
void fight(object me, object victim)
{
        object ob;
        string skill_name;
        object weapon, weapon2;
        string attack_skill;
        int double_attack;
        mapping prepare;
        string result;
        mapping my_temp;
        int i, auto_pf, pf_flag;
        string pf_skill, pf_skill1, pf_skill2;

        if( !living(me) || me->is_busy() ) return;
        if( !me->visible(victim)) return;
        if( environment(me)!=environment(victim) ) return;

        if( !mapp(my_temp = me->query_entire_temp_dbase()) )
                my_temp = ([ ]);

        auto_pf = 0;
        if( query("auto_perform", me))auto_pf=query("auto_perform", me);
        else            auto_pf = me->query_auto_perform();
        if( auto_pf < 0 ) auto_pf = 0;
        if( auto_pf > 10 ) auto_pf = 10;

        prepare = me->query_skill_prepare();
        if( sizeof(prepare) && stringp(attack_skill = (keys(prepare))[0]) ) {
                if( !query_temp("weapon", me) && (attack_skill == "sword" ||
                    attack_skill == "blade" || attack_skill == "hammer" || attack_skill == "whip") ) {
                        for (i=0; i<sizeof(keys(prepare)); i++)
                                me->prepare_skill((keys(prepare))[i]);
                        tell_object(me, HIY "你现在采用" HIR "空手技能" HIY "进行攻击，"
                                        HIY "取消所备" HIR "兵器技能" HIY "攻击。\n" NOR);
                }
        }

        if( weapon = my_temp["weapon"] )
                attack_skill=query("skill_type", weapon);
        else if( !prepare || sizeof(prepare) == 0 ) attack_skill = "unarmed";
        else if( sizeof(prepare) ) attack_skill = (keys(prepare))[0];
        if( attack_skill == "pin" ) attack_skill = "sword";

        attack_skill = me->query_skill_mapped(attack_skill);

        if( auto_pf == 1 || (random(auto_pf) == 1) ) {
                pf_flag = 0;
                if( attack_skill ) {
                        pf_skill1 = attack_skill;
                        pf_flag = 1;
                }
                if( !weapon && prepare && sizeof(prepare) > 1 )
                        pf_skill2 = me->query_skill_mapped((keys(prepare))[1]);
                if( pf_skill2 ) pf_flag = 2;
                if( pf_flag == 1 )
                        auto_perform(me, victim, pf_skill1);
                else if( pf_flag == 2 ) {
                        if( random(2) ) pf_skill = pf_skill1;
                        else pf_skill = pf_skill2;
                        if( !auto_perform(me, victim, pf_skill) )
                                auto_perform(me, victim, (pf_skill == pf_skill1) ?
                                                        pf_skill2 : pf_skill1);
                }
        }

        if( !objectp(me) || !objectp(victim) ) return;
        if( me->is_busy() ) return;

        if( attack_skill && random(me->query_dex()) >= 8 )
                double_attack = (sizeof(prepare) >= 2 && !weapon) ||
                        SKILL_D(attack_skill)->double_attack();

        // If victim is busy or unconcious, always take the chance to make an attack.
        if( victim->is_busy() || !living(victim) ) {
                set_temp("guarding", 0, me);
                if( !victim->is_fighting(me) ) victim->fight_ob(me);
                do_attack(me,victim,query_temp("weapon", me),TYPE_QUICK);

                if( me->is_fighting(victim) && victim->is_fighting(me) ) {
                        if( double_attack ) {
                                set_temp("action_flag", 1, me);
                                do_attack(me, victim, my_temp["weapon"], TYPE_QUICK);
                                set_temp("action_flag", 0, me);
                        } else // 兵器互备
                        if( objectp(weapon) &&
                            objectp(weapon2=query_temp("secondary_weapon", me)) &&
                            sizeof(me->query_skill_prepare()) > 1 &&
                            me->query_skill_prepared(query("skill_type", weapon)) &&
                            me->query_skill_prepared(query("skill_type", weapon2))) {
                                if( query_temp("hand_flag", me) ) // 上次使用的是左手
                                        do_attack(me, victim, weapon, TYPE_QUICK, 1);
                                else
                                        do_attack(me, victim, weapon2, TYPE_QUICK, 2);
                        }
                }
                /*
                if( living(victim) ) {
                        if( stringp(attack_skill) ) {
                                set_temp("action_flag", 1, me);
                                SKILL_D(attack_skill)->do_interlink(me, victim);
                                set_temp("action_flag", 0, me);
                        }
                }
                */
        // Else, see if we are brave enough to make an aggressive action.
        } else if( me->query_str() > random(victim->query_str() * 3 / 4) ) {
                set_temp("guarding", 0, me);
                if( !victim->is_fighting(me) ) victim->fight_ob(me);
                do_attack(me, victim, my_temp["weapon"], TYPE_REGULAR);

                if( me->is_fighting(victim) && victim->is_fighting(me) ) {
                        if( double_attack ) {
                                set_temp("action_flag", 1, me);
                                do_attack(me, victim, my_temp["weapon"], TYPE_REGULAR);
                                set_temp("action_flag", 0, me);
                        } else // 兵器互备
                        if( objectp(weapon) &&
                            objectp(weapon2=query_temp("secondary_weapon", me)) &&
                            sizeof(me->query_skill_prepare()) > 1 &&
                            me->query_skill_prepared(query("skill_type", weapon)) &&
                            me->query_skill_prepared(query("skill_type", weapon2))) {
                                if( query_temp("hand_flag", me) ) // 上次使用的是左手
                                        do_attack(me, victim, weapon, TYPE_REGULAR, 1);
                                else
                                        do_attack(me, victim, weapon2, TYPE_REGULAR, 2);
                        }
                }
                /*
                if( stringp(attack_skill) ) {
                        set_temp("action_flag", 1, me);
                        SKILL_D(attack_skill)->do_interlink(me, victim);
                        set_temp("action_flag", 0, me);
                }
                */

        // Else, we just start guarding.
        }else if( !query_temp("guarding", me)){
                set_temp("guarding", 1, me);
                message_combatd( guard_msg[random(sizeof(guard_msg))], me, victim, "");
                return;
        } else return;
}

//  auto_fight()
//
//  This function is to start an automatically fight. Currently this is
//  used in "aggressive", "vendetta", "hatred", "berserk" fight.
//
void auto_fight(object me, object obj, string type)
{
        // Don't let NPC autofight NPC.
        if( !playerp(me) && !playerp(obj) ) return;

        if( query_temp("owner", me) == obj ||
            obj->quety_temp("owner") == me )
                return;

        // Because most of the cases that we cannot start a fight cannot be checked
        // before we really call the kill_ob(), so we just make sure we have no
        // aggressive callout wating here.
        if( query_temp("looking_for_trouble", me))return ;
        set_temp("looking_for_trouble", 1, me);

        // This call_out gives victim a chance to slip trough the fierce guys.
        call_out("start_" + type, 0, me, obj);
}

void start_berserk(object me, object obj)
{
        int shen, sb_shen;

        if( !me || !obj ) return;               // Are we still exist( not becoming a corpse )?
        set_temp("looking_for_trouble", 0, me);

        if( me->is_fighting(obj)                // Are we busy fighting?
        ||  !living(me)                         // Are we capable for a fight?
        ||  environment(me)!=environment(obj)   // Are we still in the same room?
         || query("no_fight", environment(me))//Areweinapeaceroom?
        )   return;

        message_vision("$N用一种异样的眼神扫视著在场的每一个人。\n", me);
        if( (shen < -50000 || sb_shen>100000) && me->query_per()>random(obj->query_per())*2 &&  !wizardp(obj) ) {
                message_vision("$N对著$n喝道：" + RANK_D->query_self_rude(me)
                        + "看你实在很不顺眼，去死吧。\n", me, obj);
                me->want_kill(obj);
                me->kill_ob(obj);
                obj->fight_ob(me);
        } else {
                message_vision("$N对著$n喝道：喂！" + RANK_D->query_rude(obj)
                        + "，" + RANK_D->query_self_rude(me) + "正想找人打架，陪我玩两手吧！\n",me, obj);
                me->fight_ob(obj);
                obj->fight_ob(me);
        }
}

void start_hatred(object me, object obj)
{
        if( !me || !obj ) return;               // Are we still exist( not becoming a corpse )?
        set_temp("looking_for_trouble", 0, me);

        if( me->is_fighting(obj)                // Are we busy fighting?
        ||  !living(me)                         // Are we capable for a fight?
        ||  environment(me)!=environment(obj)   // Are we still in the same room?
         || (query("no_fight", environment(me))//Areweinapeaceroom?
        &&  !me->is_demogorgon() && !obj->is_demogorgon())
        )   return;

        if( query_temp("owner", me) == query("id", obj) ||
            query_temp("owner", obj) == query("id", me) )
                return;

        // We found our hatred!Charge!
        message_vision( catch_hunt_msg[random(sizeof(catch_hunt_msg))], me, obj);
        me->want_kill(obj);
        me->kill_ob(obj);
        obj->fight_ob(me);

        if( !playerp(me) && me->is_busy() && !intp(me->query_busy()) )
                me->interrupt_me(me);
}

void start_vendetta(object me, object obj)
{
        if( !me || !obj ) return;               // Are we still exist( not becoming a corpse )?
        set_temp("looking_for_trouble", 0, me);

        if( me->is_fighting(obj)                // Are we busy fighting?
        ||  !living(me)                         // Are we capable for a fight?
        ||  environment(me)!=environment(obj)   // Are we still in the same room?
         || query("no_fight", environment(me))//Areweinapeaceroom?
        )   return;

        // We found our vendetta opponent!Charge!
        me->want_kill(obj);
        me->kill_ob(obj);
        obj->fight_ob(me);
}

void start_aggressive(object me, object obj)
{
        if( !me || !obj ) return;               // Are we still exist( not becoming a corpse )?
        set_temp("looking_for_trouble", 0, me);

        if( me->is_fighting(obj)                // Are we busy fighting?
        ||  !living(me)                         // Are we capable for a fight?
        ||  environment(me)!=environment(obj)   // Are we still in the same room?
         || query("no_fight", environment(me))//Areweinapeaceroom?
        )   return;

        // We got a nice victim!Kill him/her/it!!!
        me->want_kill(obj);
        me->kill_ob(obj);
        obj->fight_ob(me);
}

int player_escape(object killer, object ob)
{
        object iob;
        string msg;

        if( !objectp(ob) ) return 0;
        if( !playerp(ob) ) return 0;

        if( query("combat/which_day", ob) == time()/86400 &&
            query("total_count", ob)>0 )
                return 0;

        if( !killer ) killer = ob->query_defeated_by();
        if( !killer)killer=query("query_last_damage", ob);
        if( !killer || !playerp(killer) ||
            !killer->is_want_kill(query("id", ob)) )
                return 0;

        if( query("no_newbie", ob) ||
            query("combat/WPK", ob) ||
            (query("age", ob) >= 20 &&
            query("combat_exp", ob) >= 20000000) )
                return 0;

        switch( random(7) )
        {
        case 0:
                msg = "突然只听幽幽一声长叹，一掌轻轻按来。$N大吃一惊，不及"
                      "多加思索，只是抬手一格。匆忙之间只怕对手过于厉害，难"
                      "以招架，急忙向后跃开。却见来人并不追击，只是一伸手拎"
                      "起$n，转身飘然而去，仙踪渺然。\n";
                break;

        case 1:
                msg = "$N将$n打翻在地，“哈哈”一声笑声尚未落下，只听有人冷"
                      "哼一声，忽然间掌风袭体，$N顿感呼吸不畅，几欲窒息，慌"
                      "忙中急忙退后，待得立稳脚跟，却见$n早已无影无踪。\n";
                break;

        case 2:
                msg = "一人忽然掠至，喝道：“岂有此理？我龙岛主最恨此欺善怕"
                      "恶之辈，休走！”说罢一掌击来，$N奋力招架，一招之下几"
                      "欲吐血！只见来人轻轻提起$n，冷笑两声，转身离去，$N惊"
                      "骇之下，竟然说不出一句话来。\n";
                break;

        case 3:
                msg = "突然一人喝道：“且慢！”只见一道黑影掠到，飞起一脚将"
                      "$N踢了个跟头，左手拎起$n，冷冷对$N道：“今日所幸尚未"
                      "伤人命，你作恶不甚，饶你去吧！”$N捂胸运气，不住喘息"
                      "，眼睁睁的看着来人去了。\n";
                break;

        case 4:
                msg = "$N跨前一步，忽然看到面前已多了两人，一胖一瘦，一喜一"
                      "怒，不由暗生疑窦。一人手中亮出一面铜牌，笑道：“这位" +
                      RANK_D->quert_respect(killer)+ "，这面罚恶铜牌你收下可"
                      "好？”$N听了大吃一惊，手只是一软，哪里还敢搭半句话？"
                      "那瘦子冷冷看了过来，目光如电，$N讪讪笑了两声，目送两"
                      "人带了$n逍遥而去。\n";

        case 5:
                msg = "恰在此时，正逢一老者路过，只见他微一颦眉，喝道：“兀"
                      "那" + RANK_D->query_rude(killer)+ "，伤人做甚？”$N大"
                      "怒道：“你是何人，如此嚣张？”老者大怒，一掌拍落，$N"
                      "向上只是一格，“噗噜”一下双腿陷入土中，足有半尺。老"
                      "者森然道：“我乃侠客岛木岛主是也，如有不服，恭候大驾"
                      "！”此时$N内息如狂，连句场面话也说不出来，只能眼看$n"
                      "被那木岛主带了离去。\n";
                break;

        default:
                msg = "忽听“哈哈”一阵长笑，一人道：“龙兄，想不到我们三十"
                      "年不履中土，这些武林高手却是越来越不长进了！”另一人"
                      "道：“正是，看来赏善罚恶，漫漫无期，终无尽头。”$N听"
                      "得大汗涔涔而下，环顾四方却无一人，转回头来，更是大吃"
                      "一惊！连$n也不见了。\n";
                break;
        }

        if( (environment(killer) == environment(ob)) &&
            killer->is_fighting(ob) ) {
                msg = replace_string(msg, "$n", ob->name());
                message_sort(msg, killer);
        } else {
                msg = "正逢一老者路过，见了" + ob->name() + "晕倒在地，叹口"
                      "气，将他扶起带走了。\n";
                message("vision", msg, environment(ob));
        }

        /*
        foreach (iob in all_inventory(ob))
                if( !query("money_id", iob) &&
                    !query("equipped", iob) )
                        iob->move(environment(ob));
        */

        // 初始化玩家的状态
        ob->clear_condition();
        ob->remove_killer(killer);
        killer->remove_killer(ob);

        // 通缉伤人凶手
        if( !killer->query_condition("killer") ) {
                msg = "听说官府发下海捕文书，缉拿伤人凶手" +
                             killer->name(1) + "。";
                killer->apply_condition("killer", 500);
        } else {
                msg = "听说官府加紧捉拿累次伤人的暴徒" +
                             killer->name(1) + "。";
                killer->apply_condition("killer", 800 +
                        (int)killer->query_condition("killer"));
        }
        CHANNEL_D->do_channel(this_object(), "rumor", msg);

        ob->move("/d/xiakedao/shiroom24");
        set("startroom", "/d/xiakedao/shiroom24", ob);
        ob->revive();
        set("eff_qi",query("max_qi",  ob), ob);
        set("eff_jing",query("max_jing",  ob), ob);
        set("qi", 0, ob);
        set("jing", 0, ob);

        if( intp(ob->query_busy()) )
                ob->start_busy(30);

        tell_object(ob, "你睁开眼来，看到两位老者正在静坐修炼。\n"
                    HIG "龙岛主告诉你：" + RANK_D->query_respect(ob) +
                        "，你要想离岛不妨和我说一声(ask long about 离岛)。\n" NOR);
        return 1;
}

// This function is to announce the special events of the combat.
// This should be moved to another daemon in the future.
void announce(object ob, string event)
{
        object *target;
        int i;

        switch( event ) {
        case "dead":
                message_vision(NOR "\n$N扑在地上挣扎了几下，腿一伸，口中喷出几口"
                               HIR "鲜血" NOR "，死了！\n\n" NOR, ob);
                if( environment(ob) && playerp(environment(ob)) && living(environment(ob)) )
                        tell_object(environment(ob),NOR "\n" + ob->name() + "扑腾了几下，腿一伸，口中喷出几口"
                               HIR "鲜血" NOR "，死了！\n\n" NOR);
                break;
        case "unconcious":
                message_vision("\n$N脚下一个不稳，跌在地上一动也不动了。\n\n", ob);
                player_escape(0, ob);
                break;
        case "revive":
                message_vision("\n$N慢慢睁开眼睛，清醒了过来。\n\n", ob);
                break;
        }
}

void winner_reward(object winner, object victim)
{
        object owner;
        int temp;
        int td;
        mapping today;

        owner=query_temp("owner", winner);
        if( objectp(owner) )
                winner = owner;

        winner->defeated_enemy(victim);

        if( !playerp(winner) || wizardp(victim) && 0 )
                return;

        if( !winner->is_want_kill(query("id", victim)) )
                // only if I want kill ob, I will record it.
                return;

        addn("combat/DPS", 1, winner);
        if( victim->is_not_bad())  addn("combat/DPS_NOTBAD", 1, winner);
        if( victim->is_not_good()) addn("combat/DPS_NOTGOOD", 1, winner);
        if( victim->is_bad())      addn("combat/DPS_BAD", 1, winner);
        if( victim->is_good())     addn("combat/DPS_GOOD", 1, winner);

        if( victim->query_condition("killer") )
                return;

        log_file("static/killrecord",
                 sprintf("%s %s defeat %s\n",
                         log_time(), log_id(winner), log_id(victim)));

        td = time() / 86400;
        today=query("combat/today", winner);
        if( !mapp(today) || today["which_day"] != td ) {
                today = ([ "which_day" : td,
                           "total_count" : 1,
                           query("id", victim):1,]);
        } else {
                // count how many times that winner hit the victim to unconcious
                temp=++today[query("id", victim)];
                if( query("newbie", victim)){
                        today[query("id", victim)] = temp + 1;
                        temp = today[query("id", victim)];
                }

                if( temp == query("pk_perman") ) {
                        // reach limit
                        tell_object(winner, BLINK HIR "\n今天你已经打晕" +
                                    victim->name() + chinese_number(temp) +
                                    "次了，手下留"
                                    "情吧，否则麻烦可要找上门了。\n" NOR);
                } else if( temp > query("pk_perman") )
                        // too many times
                        set("combat/need_punish", "这厮逼人太甚，真是岂有此理！", winner);

                // count how many users that winner hit to unconcious
                temp = ++today["total_count"];
                if( query("newbie", victim)){
                        today["total_count"] = temp + 5;
                        temp = today["total_count"];
                }

                if( temp == query("pk_perday") ) {
                        // reach limit
                        tell_object(winner, BLINK HIR "\n今天你已经打晕" +
                                    chinese_number(temp) + "次玩家了，手下留"
                                    "情吧，否则麻烦可要找上门了。\n" NOR);
                } else if( temp > query("pk_perday") )
                        // too many users
                        set("combat/need_punish", "丧尽天良，大肆屠戮，罪无可恕！", winner);
        }
        set("combat/today", today, winner);
}

void family_kill(object killer, object victim)
{
        // 以下为门派功绩处理
        if( !objectp(killer) || !playerp(killer) ||
            !objectp(victim) ||
            !stringp(query("family/family_name", killer)) )
                return;

        if( query("family/family_name", killer) !=
            query("family/family_name", victim) )
                return;

        // quest 任务
        if( stringp(query("quest/id", killer)) &&
            query("quest/id", killer) == query("id", victim) )
                return;

        // free 任务
        if( !playerp(victim) && query_temp("quest_ob", victim) )
                return;

        // ultra 任务
        if( !playerp(victim) && stringp(query_temp("quester", victim)) &&
            query_temp("quester", victim) == query("id", killer) )
                return;

        // ultra 任务
        if( !playerp(victim) && (query("quest_dg", killer) ||
            query("quest_hs", killer) || query("quest_sn", killer) ||
            query("quest_kh", killer)) )
                return;

        if( playerp(victim) ) {
                addn("family/gongji", -500, killer);
                tell_object(killer, sprintf("由于你残害同门弟子，师门功绩下降 %d 点。\n", 500));
        } else {
                if( killer->is_apprentice_of(victim) ) {
                        addn("family/gongji", -1000, killer);
                        tell_object(killer,sprintf("由于欺师灭组的行为，师门功绩下降 %d 点。\n", 1000));
                }
                else if( query("family/generation", killer)>query("family/generation", victim)){
                        addn("family/gongji", -600, killer);
                        tell_object(killer,sprintf("由于你残害同门尊长，师门功绩下降 %d 点。\n", 600));
                }
                else {
                        addn("family/gongji", -300, killer);
                        tell_object(killer,sprintf("由于你残害同门，师门功绩下降 %d 点。\n", 300));
                }
        }
        if( query("family/gongji", killer)<0 )
                delete("family/gongji", killer);
}

void killer_reward(object killer, object victim)
{
        object owner, charm;
        string vmark;
        string msg = "莫名其妙地死了";
        string follow_msg = 0;
        object weapon;
        mapping actions, quest, bonus;
        int shen_delta,exp_damage;
        mixed exp_lost;
        mapping ks, vs, kcombat;
        int flag, no_loss = 0;
        int exp, pot, score;
        int kill_bonus;

        if( !objectp(victim) )
                return;

        if( objectp(killer) ) {
                owner=query_temp("owner", killer);
                if( objectp(owner) )
                        killer = owner;
        }

        // This while is only to enable "break"
        vs = victim->query_entire_dbase();
        while( 1 )
        {
                if( !objectp(killer) )
                        break;

                // call the killer's override function
                killer->killed_enemy(victim);

                if( !objectp(killer) || !playerp(killer) )
                        break;

                // assure the killer's dbase
                ks = killer->query_entire_dbase();
                if( !mapp(kcombat = ks["combat"]) ) {
                        kcombat = ([ ]);
                        ks["combat"] = kcombat;
                }

                if( ks["combat_exp"]/2 < vs["combat_exp"] ) {
                        if( ks["combat_exp"] >= 100000 &&
                            ks["combat_exp"] >= vs["combat_exp"] * 2 / 3 ) {
                                // adjust experience
                                if( !ks["experience"] )
                                        ks["experience"] = 0;
                                killer->improve_experience(1 + random(5));
                        }

                        // adjust shen
                        shen_delta = -vs["shen"] / 2;
                        if( shen_delta > 100 ) {
                                // 非线性校正获得的正神
                                shen_delta = (shen_delta - 100) / 3 + 100;

                                if( shen_delta > 200 )
                                        shen_delta = (shen_delta - 200) / 3 + 200;

                                if( shen_delta > 300 )
                                        shen_delta = (shen_delta - 300) / 3 + 300;
                        } else
                        if( shen_delta < -100 ) {
                                // 非线性校正获得的负神
                                shen_delta = (shen_delta + 100) / 3 - 100;

                                if( shen_delta < -200 )
                                        shen_delta = (shen_delta + 200) / 3 - 200;

                                if( shen_delta < -300 )
                                        shen_delta = (shen_delta + 300) / 3 - 300;
                        }

                        ks["shen"] += shen_delta;
                }

                // weapon get the bonus
                if( weapon=query_temp("weapon", killer) ||
                    weapon=query_temp("armor/finger", killer) ||
                    weapon=query_temp("armor/hands", killer) )
                        weapon->killer_reward(killer, victim);

                if( weapon=query_temp("secondary_weapon", killer) )
                        weapon->killer_reward(killer, victim);

                if( weapon=query_temp("handing", killer) )
                        weapon->killer_reward(killer, victim);

                // record the user's killing
                if( playerp(victim) ) {
                        if( !intp(kcombat["PKS"]) )
                                kcombat["PKS"] = 1;
                        else
                                kcombat["PKS"]++;

                        //killer->set("pktime", (int)killer->query("mud_age"));
                        set("combat/pktime", time(), killer);

                        // do family action
                        FAMILY_D->family_kill(killer, victim);

                        // do league action
                        LEAGUE_D->league_kill(killer, victim);

                        // do bunch action
                        BUNCH_D->bunch_kill(killer, victim);

                        //if( victim->query_condition("killer") ) {
                        if( query("pk_score", victim)>0){
                                // 奖励设置
                                if( query("combat/today/"+query("id", killer), victim) )
                                        addn("pk_score", -1, victim);
                                else {
                                        kill_bonus=query("combat_exp", victim)/100;
                                        if( kill_bonus > 1000000 ) kill_bonus = 1000000;
                                        addn("combat_exp", kill_bonus, killer);
                                        tell_object(killer, "你获得了" + kill_bonus + "点经验奖励。\n");
                                }

                                set("combat/punish", time(), victim);
                                follow_msg = "听说杀人凶手" + victim->name(1) +
                                             "被" + killer->name(1) + "在" +
                                             environment(victim)->short() + HIM
                                             "就地正法了。";
                                break;
                        }

                        if( playerp(killer) && killer->in_array() && killer->query_array_status() ) {
                                follow_msg = sprintf("%s(%s)被%s(%s)带领的『%s』做掉啦！！\n",
                                                victim->name(1),
                                                capitalize(query("id", victim)),
                                                killer->query_array_master()->name(1),
                                                capitalize(query("id", killer->query_array_master())),
                                                killer->query_array_name());
                        }

                       if( playerp(killer) && killer->in_team() && objectp(killer->query_team_leader()) ) {
                                follow_msg = sprintf("%s(%s)被%s(%s)带领的队伍做掉啦！！\n",
                                                victim->name(1),
                                                capitalize(query("id", victim)),
                                                killer->query_team_leader()->name(1),
                                                capitalize(query("id", killer->query_team_leader())));
                        }

                        if( !killer->is_want_kill(vs["id"]) )
                                break;

                        if( !intp(kcombat["WPK"]) )
                                kcombat["WPK"] = 1;
                        else
                                kcombat["WPK"]++;

                        if( !killer->query_condition("killer") ) {
                                follow_msg = "听说官府发下海捕文书，缉拿杀人凶手" +
                                             killer->name(1) + "。";
                                killer->apply_condition("killer", 500);
                        } else {
                                follow_msg = "听说官府加紧捉拿累犯重案的暴徒" +
                                             killer->name(1) + "。";
                                killer->apply_condition("killer", 800 +
                                        (int)killer->query_condition("killer"));
                        }

                        addn("pk_score", 1, killer);
                        set("pk_score_time", time(), killer);
                        
                        HISTORY_D->add_history("PK_SCORE", query("id", killer), sprintf("杀死玩家 %s(%s)，年龄 %d、等级 %d，对手等级 %d\n", 
                                query("name", victim), query("id", victim), query("age", killer), query("level", killer), query("level", victim)));     
                        // be killed
                        HISTORY_D->add_history("BK_SCORE", query("id", victim), sprintf("被玩家 %s(%s)杀死，年龄 %d、等级 %d，对手等级 %d\n", 
                                query("name", killer), query("id", killer), query("age", victim), query("level", victim), query("level", killer))); 
                                   
                        // assure data of kcombat
                        if( !intp(kcombat["WPK_NOTBAD"]) ) kcombat["WPK_NOTBAD"] = 0;
                        if( !intp(kcombat["WPK_NOTGOOD"]) ) kcombat["WPK_NOTGOOD"] = 0;
                        if( !intp(kcombat["WPK_GOOD"]) ) kcombat["WPK_GOOD"] = 0;
                        if( !intp(kcombat["WPK_BAD"]) ) kcombat["WPK_BAD"] = 0;

                        // record the combat info.
                        if( victim->is_not_bad() )  kcombat["WPK_NOTBAD"]++;
                        if( victim->is_not_good() ) kcombat["WPK_NOTGOOD"]++;
                        if( victim->is_good() )     kcombat["WPK_GOOD"]++;
                        if( victim->is_bad() )      kcombat["WPK_BAD"]++;
                        break;
                } else {
                        if( mapp(query("drops", victim)) )
                                EQUIPMENT_D->killer_reward(killer,victim,query("drops", victim));

                        // 任务
                }

                if( vs["can_speak"] )
                        if( !intp(kcombat["MKS"]) )
                                kcombat["MKS"] = 1;
                        else
                                kcombat["MKS"]++;

                family_kill(killer, victim);
                quest_kill(killer, victim);

                break;
        }

        // die penalty & message of victim
        if( playerp(victim) ) {
                // clear punish info.
                delete("combat/need_punish", victim);

                // remove vendetta
                map_delete(vs, "vendetta");

                // 有tianshi charm且不被通缉死亡无损失
                if( objectp(charm = present("tianshi charm", victim)) /*&& !victim->query_condition("killer")*/ )
                {
                        set_temp("tianshi_charm", 1, victim);
                        no_loss = 1;
                }

                // 被玩家杀死且自己受到保护死亡无损失
                else
                if( objectp(killer) && playerp(killer) &&
                    (query("age", victim)<20 ||
                    query("combat_exp", victim)<20000000) &&
                    !query("no_newbie", victim) &&
                    !query("combat/WPK", victim) )
                        no_loss = 1;

                // 被玩家杀死且杀手年纪小于20岁或经验小于2000万死亡无损失
                else
                if( objectp(killer) && playerp(killer) &&
                    (query("age", killer)<20 ||
                    query("combat_exp", killer)<20000000) )
                        no_loss = 1;

                // clear all condition
                victim->clear_condition();

                if( !no_loss ) {
                        // adjust shen
                        vs["shen"] -= vs["shen"] / 10;
                        if( vs["max_neili"] >= 20 )
                                vs["max_neili"] -= 20;
                        else
                                vs["max_neili"] = 0;

                        // adjust combat exp.
                        if( !mapp(vs["combat"]) ) exp_damage = 1;
                        else exp_damage = vs["combat"]["WPK"];
                        if( exp_damage < 1 ) exp_damage = 1;
                        if( exp_damage > 5 ) exp_damage = 5;

                        // pk红名惩罚
                        if( vs["pk_score"] > 3 )
                                exp_damage += 5;
                        else if( vs["pk_score"] >= 1 )
                                exp_damage += 2;

                        exp_lost = vs["combat_exp"] * exp_damage / 5000;
                        vs["combat_exp"] -= exp_lost;

                        if( killer )
                                addn("combat/WPKEXP", exp_lost, killer);
                        addn("combat/dieexp", exp_lost, victim);
                        set("combat/last_dieexp", exp_lost, victim);

                        // adjust weiwang
                        vs["weiwang"] -= vs["weiwang"] / 10;
                        // adjust score
                        vs["score"] -= vs["score"] / 10;


                        // lost half of potential
                        if( vs["potential"] > vs["learned_points"])
                                vs["potential"] += (vs["learned_points"] - vs["potential"]) / 100;

                        // lost half of experience
                        if( vs["experience"] > vs["learned_experience"] )
                                 vs["experience"] += (vs["learned_experience"] - vs["experience"]) / 100;

                        if( !query("death", victim))flag=100;//生死玄关
                        else {
                                // 按照实战经验修正掉技能几率
                                if( query("combat_exp", victim) >= 200000000)flag=20;
                                else if( query("combat_exp", victim) >= 80000000)flag=30;
                                else if( query("combat_exp", victim) >= 50000000)flag=40;
                                else if( query("combat_exp", victim) >= 30000000)flag=50;
                                else if( query("combat_exp", victim) >= 15000000)flag=55;
                                else if( query("combat_exp", victim) >= 8000000)flag=60;
                                else if( query("combat_exp", victim) >= 5000000)flag=65;
                                else                                               flag = 70;
                        }
                        if( random(100) < flag ) victim->skill_death_penalty();
                }

                if( objectp(charm) && no_loss )
                        destruct(charm);

                // generate message of die reason
                if( objectp(killer) && killer->is_character() ) {
                        msg = "被" + filter_color(killer->name(1));

                        if( query("race", killer) == "野兽"
                         || query("race", killer) == "蛇类"
                         || query("race", killer) == "游鱼"
                         || query("race", killer) == "昆虫")msg+="咬死了";
                        else if( query("race", killer) == "家畜")msg+="踩死了";
                        else if( query("race", killer) == "飞禽")msg+="啄死了";
                        else {
                                if( query_temp("hand_flag", killer) )
                                        actions = query("second_actions", killer);
                                else
                                        actions = query("actions", killer);
                                switch (mapp(actions) ? actions["damage_type"] : "杀伤") {
                                case "擦伤":
                                case "割伤":
                                        msg+="砍死了";
                                        break;
                                case "刺伤":
                                        msg+="刺死了";
                                        break;
                                case "震伤":
                                case "瘀伤":
                                        msg+="击死了";
                                        break;
                                case "内伤":
                                        msg+="震死了";
                                        break;
                                default:
                                        msg+="杀死了";
                                }
                        }
                }

                if( stringp(query_temp("die_reason", victim)) )
                        msg=filter_color(query_temp("die_reason", victim));

                delete_temp("die_reason", victim);
                CHANNEL_D->channel_broadcast("combat",
                        sprintf("听说%s" HIR + msg + "。", victim->name(1)));

                // show the follow msg after victim die
                if( follow_msg )
                        CHANNEL_D->channel_broadcast("rumor",
                                              follow_msg);

                set("combat/last_die", filter_color(msg), victim);
                if( objectp(killer) && playerp(killer) && playerp(victim) ) {
                        string cname;
                        object couple;

                        log_file("static/killrecord",
                                 sprintf("%s %s killed %s\n",
                                         log_time(),
                                         log_id(killer), log_id(victim)));

                        killer->remove_killer(victim);

                        // check sadly
                        cname=query("couple/couple_id", victim);
                        if( cname &&
                            query("gender", victim) == "女性" &&
                            query("combat_exp", victim) >= 970000 &&
                            objectp(couple = find_player(cname)) &&
                            couple != killer &&
                            query("couple/married", couple) <= 1 &&
                            query("couple/married", victim) <= 1){
                                addn("static/sadly", 1, couple);
                                tell_object(couple, "你的妻子死在了" +
                                            killer->name(1) + "的手下" +
                                            "，你感到了一阵深深的哀伤。\n");
                        }
                }
        }

        if( stringp(vmark=query("vendetta_mark", victim)) && objectp(killer) )
                addn("vendetta/"+vmark, 1, killer);
}

// me hit victim with poison in ob
void hit_with_poison(object me, object victim, object ob)
{
        mapping p, ap;
        string msg;
        string poison_type;

        if( !mapp(p=query_temp("daub/poison", ob)) )
                return;

        if( !stringp(poison_type=query_temp("daub/poison_type", ob)) )
                return;

        if( !p["remain"] )
                return;

        // affect parameter
        ap = allocate_mapping(4);
        if( ob == me )
                ap["level"] = p["level"] * 9 / 10 + 1;
        else
                ap["level"] = p["level"] * 8 / 10 + 1;

        ap["id"]       = p["id"];
        ap["name"]     = p["name"];
        ap["duration"] = 1;

        if( p["remain"] > p["level"] ) {
                // decrase the remain poison
                p["remain"] -= p["level"];
        } else {
                // the poison has run out
                delete_temp("daub", ob);
        }

        msg = HIG "突然$n觉得受伤的地方有一些麻痒";
        if( p["id"] == query("id", victim)){
                if( !query_temp("has_announce/defense1", victim)){
                        set_temp("has_announce/defense1", 1, victim);
                        victim->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", victim :), victim), 15);
                        msg += "。\n" NOR HIC "$n默一运功，冷笑两声：“好你个" +
                               RANK_D->query_rude(me) + "，居然在我面前卖弄" +
                               RANK_D->query_self_rude(victim) +
                               "毒药！真是假李鬼碰上真李逵了！”\n" NOR;
                } else
                        msg += "，不过$n并没有在意。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        if( p["level"] < victim->query_skill("poison", 1) ) {
                if( !query_temp("has_announce/defense2", victim)){
                        set_temp("has_announce/defense2", 1, victim);
                        victim->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", victim :), victim), 15);
                        msg += "。\n" NOR HIC "$n呼吸数次，嘿然冷笑道："
                               "“米粒之珠，也放光华？”\n" NOR;
                } else
                        msg += "，不过$n显然并没有把它放在心上。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        if( p["level"] < victim->query_skill("force") / 2 ) {
                if( !query_temp("has_announce/defense3", victim)){
                        set_temp("has_announce/defense3", 1, victim);
                        victim->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", victim :), victim), 15);
                        msg += "。\n" NOR HIC "$n深吸一口气，哈哈长笑道：“好家伙，居然" +
                               (ob == me ? "在身上淬毒" : "使用淬毒兵器") +
                               "，你这些下三滥的手段也敢到我面前卖弄？”\n" NOR;
                } else
                        msg += "，不过$n看起来似乎并无大碍。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        victim->affect_by(poison_type, ap);

        msg += "，大概是中毒了。\n" NOR;
        if( p["level"] < victim->query_skill("force") ) {
                msg += HIG "$n深深吸一了口气，面目凝重，手中的攻势陡然一紧。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        msg += HIR "$n闷哼一声，攻势顿缓，神情甚是痛苦。\n" NOR;
        if( !victim->is_busy() )
                victim->start_busy(1 + random(2));
        set_ahinfo(msg);
        return;
}

// me hit victim, but poison by victim in ob
void hit_poison(object me, object victim, object ob)
{
        mapping p, ap;
        string msg;
        string poison_type;

        if( !mapp(p=query_temp("daub/poison", ob)) )
                return;

        if( !stringp(poison_type=query_temp("daub/poison_type", ob)) )
                return;

        if( !p["remain"] )
                return;

        if( query_temp("armor/hands", me) )
                return;

        // affect parameter
        ap = allocate_mapping(4);
        ap["level"]    = p["level"] * 7 / 10 + 1;

        ap["id"]       = p["id"];
        ap["name"]     = p["name"];
        ap["duration"] = 1;

        if( p["remain"] > p["level"] ) {
                // decrase the remain poison
                p["remain"] -= p["level"];
        } else {
                // the poison has run out
                delete_temp("daub", ob);
        }

        msg = HIG "突然$N觉得浑身有些麻痒";
        if( p["id"] == query("id", me)){
                if( !query_temp("has_announce/defense1", me)){
                        set_temp("has_announce/defense1", 1, me);
                        me->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", me :), me), 15);
                        msg += "。\n" NOR HIC "$N默一运功，冷笑两声：“好你个" +
                               RANK_D->query_rude(victim) + "，居然在我面前卖弄" +
                               RANK_D->query_self_rude(victim) +
                               "的毒药！把你衣服上的毒药都还给我！”\n" NOR;
                } else
                        msg += "，不过$N并没有在意。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        if( p["level"] < me->query_skill("poison", 1) ) {
                if( !query_temp("has_announce/defense2", me)){
                        set_temp("has_announce/defense2", 1, me);
                        me->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", me :), me), 15);
                        msg += "。\n" NOR HIC "$N呼吸数次，嘿然冷笑道：“米粒之珠，也"
                               "放光华？你在衣衫上淬毒我就怕你了？”\n" NOR;
                } else
                        msg += "，不过$N显然并没有把它放在心上。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        if( p["level"] < me->query_skill("force") / 2 ) {
                if( !query_temp("has_announce/defense3", me)){
                        set_temp("has_announce/defense3", 1, me);
                        me->start_call_out(bind((: call_other,
                                __FILE__, "clear_announce", me :), me), 15);
                        msg += "。\n" NOR HIC "$N深吸一口气，哈哈长笑道：“你居然在衣衫上"
                               "淬毒，这些下三滥的手段也敢到我面前卖弄？”\n" NOR;
                } else
                        msg += "，不过$N看起来似乎并无大碍。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        me->affect_by(poison_type, ap);

        msg += "，看来有些不妙。\n" NOR;
        if( p["level"] < me->query_skill("force") ) {
                msg += HIG "$N深深吸一了口气，面目凝重，手中的攻势陡然一紧。\n" NOR;
                set_ahinfo(msg);
                return;
        }

        msg += HIR "$N脚步一个跄踉，攻势顿缓，神色难看之极。\n" NOR;
        me->start_busy(1 + random(2));
        set_ahinfo(msg);
        return;
}

void clear_announce(object me)
{
        if( !objectp(me) )
                return;

        delete_temp("has_announce", me);
}

protected void quest_kill(object killer, object victim)
{
        mapping quest,b;
        int level,i,n,exp,pot,score,weiwang,mar,quest_count,all_quest;
        mixed special = 0;
        mixed money = 0;
        object ob,*all_killer,obj;
        object tongren;

        // 以下是NPC帮派任务的奖励
        // 如果是追杀任务
        if( !playerp(victim) && query("bunch_quest", victim)){
                quest=query("bunch_quest", victim);
                if( quest["type"] == "追杀" &&
                    ((quest["bunch_type"] == "good" && good_bunch(killer) && query("shen", killer) >= 0) ||
                    (quest["bunch_type"] == "bad" && bad_bunch(killer) && query("shen", killer) <= 0)) &&
                    query("combat_exp", killer)<270000000){
                        exp = quest["bonus"];
                        exp += 120;
                        exp += random(exp/2);
                        pot = exp/(2 + random(2));
                        score = 25 + random(20);
                        weiwang = 25 + random(20);

                        b = ([ "exp" : exp,
                               "pot" : pot,
                               "score" : score,
                               "weiwang" : weiwang,
                               "prompt": "在追杀" + victim->name() + HIG "的过程中，经过锻炼"
                        ]);

                        GIFT_D->delay_bonus(killer, b);
                }
                return;
        }

        // 如果是铲除外敌任务
        if( query("bunch_quest", killer) &&
            query("bunch_quest", killer)["type"] == "铲除外敌" &&
            query("bunch_quest", killer)["ob"] == victim){
                quest=query("bunch_quest", killer);

                exp = quest["bonus"];
                exp += 120;
                exp += random(exp/2);
                pot = exp/(2 + random(2));
                score = 25 + random(20);
                weiwang = 25 + random(20);

                b = ([ "exp" : exp,
                       "pot" : pot,
                       "score" : score,
                       "weiwang" : weiwang,
                       "prompt": "在铲除外来敌人" + victim->name() + HIG "的过程中，经过锻炼"
                ]);

                GIFT_D->delay_bonus(killer, b);

                delete("bunch_quest", killer);
                addn_temp("finish_bunch_times", 1, killer);
                return;
        }

        // 以下是组队任务的奖励
        if( !playerp(victim) && query("my_type", victim)){
                /*
                if( objectp(victim->query_defeated_by()) &&
                    playerp(victim->query_defeated_by()) )
                    killer = victim->query_defeated_by();
                */
                switch(query("my_type", victim) )
                {
                case "外敌":
                        GROUP_QUEST_D->be_killed(1);
                        GROUP_QUEST_D->add_killer(killer);
                        exp=query("bonus", victim);
                        pot = exp;
                        score = random(exp);
                        weiwang = exp + random(exp);
                        mar = random(exp/2);
                        if( GROUP_QUEST_D->query_drop() < 2 && random(10) == 1 ) {
                                GROUP_QUEST_D->add_drop(1);
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);
                        }
                        GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                 "weiwang" : weiwang, "score" : score ]), 1);
                        if( GROUP_QUEST_D->query_enemy() == 0 ) {
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);

                                all_killer = GROUP_QUEST_D->query_killer();
                                n = sizeof(all_killer);
                                exp = 2000+20*n;
                                pot = exp;
                                score = 1000;
                                weiwang = 3000;
                                mar = 500;
                                foreach (ob in all_killer)
                                {
                                        if( !objectp(ob) || !playerp(ob) )
                                                continue;

                                        tell_object(ob, HIW "所有入侵的敌人都给打退了，由于你也参与了战斗，" +
                                                            "因此现在你也一起分享胜利果实！\n" NOR);
                                        GIFT_D->bonus(ob, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                             "weiwang" : weiwang, "score" : score ]), 1);
                                }
                        }
                        break;
                case "门派":
                        GROUP_QUEST_D->be_killed(1);
                        if( query("门派", victim) == query("family/family_name", killer)){
                                GROUP_QUEST_D->add_killer(killer);
                                exp=query("bonus", victim);
                                pot = exp;
                                score = random(exp);
                                weiwang = exp + random(exp);
                                mar = random(exp/2);
                        if( GROUP_QUEST_D->query_drop() < 2 && random(10) == 1 ) {
                                GROUP_QUEST_D->add_drop(1);
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);
                                }
                                GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                         "gongxian" : weiwang, "score" : score, "weiwang" : weiwang ]), 1);
                        }
                        if( GROUP_QUEST_D->query_enemy() == 0 ) {
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);
                                all_killer = GROUP_QUEST_D->query_killer();
                                n = sizeof(all_killer);
                                exp = 2000+20*n;
                                pot = exp;
                                score = 1000;
                                weiwang = 30000;
                                mar = 500;
                                foreach (ob in all_killer)
                                {
                                        if( !objectp(ob) || !playerp(ob))
                                                continue;

                                        tell_object(ob, HIW "所有入侵的敌人都给打退了，由于你也参与了战斗，" +
                                                            "因此现在你也一起分享胜利果实！\n" NOR);
                                        GIFT_D->bonus(ob, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                             "gongxian" : weiwang, "score" : score, "weiwang" : weiwang ]), 1);
                                }
                        }
                        break;
                case "帮派":
                        GROUP_QUEST_D->be_killed(1);
                        if( query("帮派", victim) == query("bunch/bunch_name", killer)){
                                GROUP_QUEST_D->add_killer(killer);
                                exp=query("bonus", victim);
                                pot = exp;
                                score = random(exp);
                                weiwang = exp + random(exp);
                                mar = random(exp/2);
                                if( GROUP_QUEST_D->query_drop() < 2 && random(10) == 1 ) {
                                GROUP_QUEST_D->add_drop(1);
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);
                                }
                                GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                   "weiwang" : weiwang, "score" : score ]), 1);
                        }
                        if( GROUP_QUEST_D->query_enemy() == 0 ) {
                                tongren = new("/clone/tongren/tongren"+(1+random(2)));
                                message_vision(HIR "叮~~一声，从$N" HIR "掉出一样东西，$n" HIR
                                               "赶紧拣了起来。\n" NOR, victim, killer);
                                tell_object(killer, BLINK + HIG "你得到了" + tongren->name() + BLINK + HIG "。\n" NOR);
                                tongren->move(killer, 1);
                                all_killer = GROUP_QUEST_D->query_killer();
                                n = sizeof(all_killer);
                                exp = 2000+20*n;
                                pot = exp;
                                score = 1000;
                                weiwang = 3000;
                                mar = 500;
                                foreach (ob in all_killer)
                                {
                                        if( !objectp(ob) || !playerp(ob) )
                                                continue;

                                        tell_object(ob, HIW "所有入侵的敌人都给打退了，由于你也参与了战斗，" +
                                                            "因此现在你也一起分享胜利果实！\n" NOR);
                                        GIFT_D->bonus(ob, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                             "weiwang" : weiwang, "score" : score ]), 1);
                                }
                        }
                        break;
                }
                return;
        }

        // 以下是宗师任务的奖励
        if( mapp(quest=query("quest_kh", killer)) && quest["desc"] == "灭"){//葵花太监
                if( (quest["name"] == "男子" && query("gender", victim) == "男性") ||
                    (quest["name"] == "女子" && query("gender", victim) == "女性") ||
                    (quest["type"] == "地点" && (domain_file(base_name(victim)) == quest["dir"] ||
                    (stringp(query("startroom", victim)) && (domain_file(query("startroom", victim)) == quest["dir"])) ||
                     base_dir(victim) == quest["dir1"])) ) {
                        if( (quest["time"]-time()) < 0 ) {
                                delete("quest_kh", killer);
                                return ;
                        }
                        else if( query("race", victim) == "野兽")return ;
                        else {
                                level = (quest["level"]+1)*(70+random(20));
                                exp = level;
                                pot = exp/(2+random(2));
                                score = random(exp/20);
                                weiwang = random(exp/15);
                                if( query("questkh_times", killer)+1>200 )
                                        mar=exp/5+random(query("questkh_times", killer));
                                else mar = 20 + random(20);
                                if( mar > 1000 ) mar = 1000;

                                GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                         "weiwang" : weiwang, "score" : score ]), 1);

                                return ;
                        }
                }
        }

        if( mapp(quest=query("quest_dg", killer))){//独孤求败
                if( quest["type"] == "杀" && victim->name(1) == quest["name"] ) {
                        if( (i=query("questdg_times", killer))<8 )
                                i = 15-i;
                        else
                                i = 1;
                        level = quest["level"]+1;
                        exp = level*105 + quest["bonus"]*3;
                        exp += random(exp/2);
                        exp = exp/i;
                        /*
                        if( level == 9 )
                                exp+=query("questdg_times", killer);
                        if( level == 9)
                                pot = exp*(2+random(2));
                        else
                        */
                                pot = exp/(2+random(2));
                        score = random(exp/20);
                        weiwang = random(exp/15);

                        if( query("questdg_times", killer)+1>200 )
                                mar=exp/5+random(query("questdg_times", killer));
                        else mar = 20 + random(20);
                        if( mar > 1000 ) mar = 1000;

                        GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                 "weiwang" : weiwang, "score" : score ]), 1);

                        addn("questdg_times", 1, killer);

                        quest_count=query("questdg_times", killer)%500;
                        if( quest_count == 50 )
                                special = 1;
                        else if( quest_count == 100)
                                special = "/clone/gift/puti-zi";
                        else if( quest_count == 150)
                                money = 1;
                        else if( quest_count == 200)
                                money = 1;
                        else if( quest_count == 250)
                                money = 1;
                        else if( quest_count == 300)
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 350)
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 400)
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 450)
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 0)
                                special = "/clone/gift/jiuzhuan";

                        if( special ) GIFT_D->special_bonus(victim, killer, special);
                        if( money ) QUEST_D->money_bonus(victim, killer, money);

                        addn("total_hatred", -5, killer);
                        if( query("total_hatred", killer) < 0 ) set("total_hatred", 0, killer);
                        delete("quest_dg", killer);

#ifdef DB_SAVE
                        if( MEMBER_D->is_valid_member(query("id", killer))){
                                if( query("special_skill/sophistry", killer)){
                                        if( query("questdg_times", killer) >= 5000 )
                                                set("questdg_times", 0, killer);
                                } else {
                                        if( query("questdg_times", killer) >= 2000 )
                                                set("questdg_times", 0, killer);
                                }
                        } else {
#endif
                                if( query("questdg_times", killer) >= 1000 )
                                        set("questdg_times", 0, killer);
#ifdef DB_SAVE
                        }
#endif
                        all_quest=query("all_quest_dg", killer);
                        if( !all_quest ) all_quest = 0;
                        all_quest += 1;
                        if( all_quest == 1000 ) {
                                obj = new("/clone/gift/xuanhuang");
                                tell_object(killer,"由于你已经累计完成了一千个任务，因此你得到了一个" +
                                                query("name", obj)+"作为奖励\n");
                                if( !obj->move(killer) )
                                        obj->move(environment(killer));
                                set("all_quest_dg", 0, killer);
                        } else
                                set("all_quest_dg", all_quest, killer);
                        return ;
                }
        }

        if( mapp(quest=query("quest_hs", killer))){//黄裳
                if( quest["type"]=="杀"&&quest["name"]==victim->name(1) ) {
                        tell_object(killer,"恭喜你！你又为绿林立了一功！\n");
                        if( (i=query("quesths_times", killer))<15 )
                                i = 15-i;
                        else
                                i = 1;
                        level = quest["level"]+1;
                        exp = level*80 + quest["bonus"]*3;
                        exp += random(exp/2);
                        exp = exp/i;
                        if( level > 4 && i > 5 )
                                exp /= 2;
                        if( level == 9 )
                                exp+=query("quesths_times", killer);
                        /*
                        if( level == 9 )
                                pot = exp*(2+random(2));
                        else
                        */
                                pot = exp/(2+random(2));
                        score = random(exp/20);
                        weiwang = random(exp/15);
                        if( query("quesths_times", killer)+1>200 )
                                mar=exp/5+random(query("quesths_times", killer));
                        else mar = 20 + random(20);
                        if( mar > 1000 ) mar = 1000;

                        GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                 "weiwang" : weiwang, "score" : score ]), 1);

                        addn("quesths_times", 1, killer);
                        quest_count=query("quesths_times", killer)%500;
                        if( quest_count == 50)
                                special = 1;
                        else if( quest_count == 100 )
                                special = "/clone/gift/puti-zi";
                        else if( quest_count == 150 )
                                money = 1;
                        else if( quest_count == 200 )
                                money = 1;
                        else if( quest_count == 250 )
                                money = 1;
                        else if( quest_count == 300 )
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 350 )
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 400 )
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 450 )
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 0 )
                                special = "/clone/gift/jiuzhuan";

                        if( special ) GIFT_D->special_bonus(victim, killer, special);
                        if( money ) QUEST_D->money_bonus(victim, killer, money);

                        addn("total_hatred", -5, killer);
                        if( query("total_hatred", killer) < 0 ) set("total_hatred", 0, killer);
                        delete("quest_hs", killer);
                        set_temp("hs_giveup", 0, killer);

#ifdef DB_SAVE
                        if( MEMBER_D->is_valid_member(query("id", killer))){
                                if( query("special_skill/sophistry", killer)){
                                        if( query("quesths_times", killer) >= 5000 )
                                                set("quesths_times", 0, killer);
                                } else {
                                        if( query("quesths_times", killer) >= 2000 )
                                                set("quesths_times", 0, killer);
                                }
                        } else {
#endif
                                if( query("quesths_times", killer) >= 1000 )
                                        set("quesths_times", 0, killer);
#ifdef DB_SAVE
                        }
#endif

                        all_quest = (int)query("all_quest_hs", killer);
                        if( !all_quest ) all_quest = 0;
                        all_quest += 1;
                        if( all_quest == 1000 ) {
                                obj = new("/clone/gift/xuanhuang");
                                tell_object(killer,"由于你已经累计完成了一千个任务，因此你得到了一个" +
                                                query("name", obj)+"作为奖励\n");
                                if( !obj->move(killer) )
                                        obj->move(environment(killer));
                                set("all_quest_hs", 0, killer);
                        } else
                                set("all_quest_hs", all_quest, killer);
                        return ;
                }
        }

        if( mapp(quest=query("quest_sn", killer))){//南海神尼
                if( quest["type"]=="杀"&&quest["name"]==victim->name(1) ) {
                        tell_object(killer,"恭喜你！你又为南海神尼立了一功！\n");
                        if( (i=query("questsn_times", killer))<15 )
                                i = 15-i;
                        else i = 1;
                        level = quest["level"]+1;
                        exp = level*80 + quest["bonus"]*3;
                        exp += random(exp/2);
                        exp = exp/i;
                        if( level > 4 && i > 5 )
                                exp /= 2;
                        if( level == 9 )
                                exp+=query("questsn_times", killer);
                        /*
                        if( level == 9 )
                                pot = exp*(2+random(2));
                        else
                        */
                                pot = exp/(2+random(2));
                        score = random(exp/20);
                        weiwang = random(exp/15);
                        if( query("questsn_times", killer)+1>200 )
                                mar=exp/5+random(query("questsn_times", killer));
                        else mar = 20 + random(20);
                        if( mar > 1000 ) mar = 1000;

                        GIFT_D->bonus(killer, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                 "weiwang" : weiwang, "score" : score ]), 1);

                        addn("questsn_times", 1, killer);
                        quest_count=query("questsn_times", killer)%500;
                        if( quest_count == 50 )
                                special = 1;
                        else if( quest_count == 100 )
                                special = "/clone/gift/puti-zi";
                        else if( quest_count == 150 )
                                money = 1;
                        else if( quest_count == 200 )
                                money = 1;
                        else if( quest_count == 250 )
                                money = 1;
                        else if( quest_count == 300 )
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 350 )
                                special = "/clone/gift/tianxiang";
                        else if( quest_count == 400 )
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 450 )
                                special = "/clone/gift/jiuzhuan";
                        else if( quest_count == 0 )
                                special = "/clone/gift/jiuzhuan";

                        if( special ) GIFT_D->special_bonus(victim, killer, special);
                        if( money ) QUEST_D->money_bonus(victim, killer, money);

                        addn("total_hatred", -5, killer);
                        if( query("total_hatred", killer) < 0 ) set("total_hatred", 0, killer);
                        delete("quest_sn", killer);
                        set_temp("sn_giveup", 0, killer);

#ifdef DB_SAVE
                        if( MEMBER_D->is_valid_member(query("id", killer))){
                                if( query("special_skill/sophistry", killer)){
                                        if( query("questsn_times", killer) >= 5000 )
                                                set("questsn_times", 0, killer);
                                } else {
                                        if( query("questsn_times", killer) >= 2000 )
                                                set("questsn_times", 0, killer);
                                }
                        } else {
#endif
                                if( query("questsn_times", killer) >= 1000 )
                                        set("questsn_times", 0, killer);
#ifdef DB_SAVE
                        }
#endif

                        all_quest = (int)query("all_quest_sn", killer);
                        if( !all_quest ) all_quest = 0;
                        all_quest += 1;
                        if( all_quest == 1000 ) {
                                obj = new("/clone/gift/xuanhuang");
                                tell_object(killer,"由于你已经累计完成了一千个任务，因此你得到了一个" +
                                                query("name", obj)+"作为奖励\n");
                                if( !obj->move(killer) )
                                        obj->move(environment(killer));
                                set("all_quest_sn", 0, killer);
                        } else
                                set("all_quest_sn", all_quest, killer);
                        return ;
                }
        }
}
