// �����Ʒ� -- ������һ
// Modified by snowman@SJ 19/12/2000

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
string perform_name(){ return HBBLU"������һ"NOR; }
int perform(object me, object target)
{
        int j;

        if( !target ) target = offensive_target(me);

        if( !target 
          || !me->is_fighting(target)
          || !living(target)
          || environment(target)!= environment(me))
                return notify_fail("��������һ��ֻ����ս���жԶ���ʹ�á�\n");

        if( me->query_temp("weapon") )
                return notify_fail("�������ֲ���ʹ�á�������һ����\n");

        if( (int)me->query_skill("super-strike", 1) < 200 )
                return notify_fail("��������Ʒ���������죬ʹ������������һ��������\n");

        if( (int)me->query_skill("force", 1) < 200 )
                return notify_fail("��Ĺ�Ԫ���ɷ��ȼ�������ʹ������������һ��������\n");

        if( (int)me->query_skill("force") < 230 )
                return notify_fail("����ڹ��ȼ�����������ʹ�á�������һ����\n");

        if( (int)me->query_str() < 33 )
                return notify_fail("�������������ǿ����ʹ������������һ������\n");

        if (me->query_skill_prepared("strike") != "super-strike"
         || me->query_skill_mapped("strike") != "super-strike"
         || me->query_skill_mapped("parry") != "super-strike")
                return notify_fail("�������޷�ʹ�á�������һ����\n");    
        
        if( (int)me->query("max_neili") < 1500)
                return notify_fail("����������̫����ʹ������������һ����\n");      

        if( (int)me->query("neili") < 1000 )
                return notify_fail("����������̫����ʹ������������һ����\n");

        if((int)me->query_temp("tzzf") && userp(me)) 
                return notify_fail("������ʹ���Ƶ���\n");

	j = me->query_skill("super-strike", 1)/3;
        message_vision(HIW"\n$N����������ͻȻ����΢�࣬�����������ͽ�ֱ��$n��\n"NOR, me, target);

        me->add_temp("apply/strike", j);
        me->add_temp("apply/damage", j/2);
        me->add_temp("apply/attack", j);
        me->set_temp("tz/heyi", 1);
        COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
        me->set_temp("tz/heyi", 2);
        if(me->is_fighting(target)){
          	if(random(me->query("combat_exp",1)) > target->query("combat_exp", 1)/3)
            		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
          	else
            		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 2);
        }
        me->delete_temp("tz/heyi");
        me->add_temp("apply/strike", -j);
        me->add_temp("apply/damage", -j/2);
        me->add_temp("apply/attack", -j);
        if( j * 4 > 250 && random(2) ){
        	tell_object(me, HIY"\n������������һ�䣬��ʼ�����ش��ġ�������������\n"NOR);
        	me->start_call_out( (: call_other, __DIR__"tianlei", "tianlei_hit", me, 2 :), 1 );
        }
        me->add("neili", -500);
        me->add("jingli", -100);
        me->start_perform(5,"��������һ��");
        return 1;
}

int help(object me)
{
        write(WHT"\n�����Ʒ���������һ����"NOR"\n");
        write(@HELP
        ������ǧ�����ƹ���ʮ������֮һ��������������һ���������Ͷ��ޱȡ�
        ���Ҿݰ��л���˵�������������ڴ���֮�����һʽɱ�У�
        
        Ҫ��  ���� 1000 ���ϣ�      
                ������� 1500 ���ϣ�
                ���� 100 ���ϣ�  
                �������� 33 ���ϣ�
                �����Ʒ��ȼ� 140 ���ϣ�
                ��Ԫ���ɷ��ȼ� 140 ���ϣ�    
                �����ޱ�����
HELP
        );
        return 1;
}