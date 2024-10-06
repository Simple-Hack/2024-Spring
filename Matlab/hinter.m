% ���庯��shier
function xt = shier(~, x)
    k1 = 1; k2 = 0.5; b = 0.1; c = 0.02;
    xt = [x(1)*(k1 - b*x(2)); x(2)*(-k2 + c*x(1))];
end

% ��������ռ��еı���
clear

% �趨ʱ�䷶Χ�ͳ�ʼ����
ts = linspace(0, 15, 151); % ʹ��linspace���ɵȼ���ʱ���
x0 = [25; 2];

% ����ode45���΢�ַ���
[t, x] = ode45(@shier, ts, x0);

% ��������������ͼ�Ĵ���
figure;
subplot(2,2,1)
% �����ͼ��ʱ����Ӧͼ
plot(t, x(:, 1), t, x(:, 2)) % ��������״̬������ʱ������
grid on % ���������
title('Time Domain Response') % ��ӱ���
xlabel('Time') % ����x���ǩ
ylabel('State Variables') % ����y���ǩ
%legend('x_1(t)', 'x_2(t)') % ���ͼ��
% �ұ���ͼ����ƽ��ͼ
subplot(2,2,2);
plot(x(:, 1), x(:, 2)) % ������ƽ��ͼ
grid on % ���������
title('Phase Plane') % ��ӱ���
xlabel('x_1') % ����x���ǩ
ylabel('x_2') % ����y���ǩ

subplot(2,2,[3,4])
grid on 
x5=linspace(-2*pi,2*pi,100)
y5=cos(2*x5)
plot(x5,y5)
