inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{

        if ((int)me->query_skill("force", 1) < 10)
                return notify_fail("��Ļ����ڹ���򻹲�����������ѧϰʥ���ķ���\n");

        return ::valid_learn(me);
}

int valid_force(string force)
{
        // return (force == "shenghuo-shengong");
        return 1;
}

int practice_skill(object me)
{
        return notify_fail("ʥ���ķ�ֻ����ѧ(learn)�������������ȡ�\n");
}


string exert_function_file(string func)
{
        return __DIR__"shenghuo-xinfa/" + func;
}