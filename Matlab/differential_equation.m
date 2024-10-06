function dydt = df4(x,y)
    dydt= zeros(2,1);
    dydt(1)=y(2);
    dydt(2)=-y(1)-sin(2*x);
end

df4_handle = @df4; % ����һ�������������ָ��df4����
[t,y] = ode45(df4_handle,[pi,2*pi],[1,1]); % ʹ�ú������������Ϊode45�ĵ�һ������

% ���ƽ��ͼ��
plot(t,y(:,1),'r',t,y(:,2),'b')
xlabel('t')
ylabel('y')
legend('y^','y^^')
title('Solution of df4')