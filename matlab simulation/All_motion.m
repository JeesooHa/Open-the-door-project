clear all
%Run Robotics ToolBox
startup_rvc


IT = 0;
FT = 2;
t = [IT:0.05:FT];
time = 0;
s_time = 0.05;
angle = linspace(0,pi*3.7/18,25);
angle_b = linspace(pi*3.7/18,0,25);
n = 1;
%%%%%
q1 = 0.01;%pi/180;
q2 = pi/4;
q3 = pi*3/4;
q4 = 0.01;%pi/180;
q5 = 0.01;%pi/180;
q6 = 0.01;%pi/180;
Qs = [q1 q2 q3 q4 q5 q6];
[xcy ycy zcy] = cylinder([0.02 0.02])


%Desired configurations position velocity acceleration
%%%%%
home = [-0.3775;0;0.2475];
home_d = [0;0;0];
home_dd = [0;0;0];
home_A = [0;0;0];
home_Ad = [0;0;0];
home_Add = [0;0;0];
task1 = [-0.45+0.01;0.34;0.355];
task1_d = [0;0;0];
task1_dd = [0;0;0];
task1_A = [-pi/2;0;0];
task1_Ad = [0;0;0];
task1_Add = [0;0;0];
task2 = [-0.45+0.01;0.34+0.05;0.355];
task2_d = [0;0;0];
task2_dd = [0;0;0];
task2_A = [-pi/2;0;0];
task2_Ad = [0;0;0];
task2_Add = [0;0;0];
open = [-0.45+0.01;0.34+0.05;0.355];
open_d = [0;0;0];
open_dd = [0;0;0];
open_A = [-pi/2;0;0];
open_Ad = [0;0;0];
open_Add = [0;0;0];
E_open = [-0.45+0.01;-0.34+0.05;0.355];
E_open_d = [0;0;0];
E_open_dd = [0;0;0];
E_open_A = [-pi/2;0;pi/2];
E_open_Ad = [0;0;0];
E_open_Add = [0;0;0];
%%%%%
B_open = [-0.15+0.01;-0.34+0.05;0.355];
B_open_d = [0;0;0];
B_open_dd = [0;0;0];
B_open_A = [-pi/2;0;pi/2];
B_open_Ad = [0;0;0];
B_open_Add = [0;0;0];


for i=1 : 25
p2top3(:,:,i) = [-0.45+0.01 ; +0.34+0.05-0.115*cos(angle(i)) ; 0.355-0.115*sin(angle(i))];
a2toa3(:,:,i) = [pi/2 + angle(i) ; 0 ; 0];
end
for i=1 : 25
p3top4(:,:,i) = [-0.45+0.01 ; +0.34+0.05-0.115*cos(angle_b(i)) ; 0.355-0.115*sin(angle_b(i))];
a3toa4(:,:,i) = [pi/2 + angle_b(i) ; 0 ; 0];
end


% trajectory variables
cnst_h2t1 = [0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0];
cnst_h2t1_A =[0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
cnst_t12t2 = [0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
cnst_h2t1_A =[0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
cnst_t12t2_A =[0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
cnst_t22t3 = [0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
          
cnst_o2E = [0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0];
cnst_o2E_A =[0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
%%%%%
cnst_E2B = [0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0];
cnst_E2B_A = [0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0];
%%%%%          
cnst_B2h = [0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0;
            0 0 0];
cnst_B2h_A =[0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0;
              0 0 0];
          
htot1 = home;
htot1_d = home_d;
htot1_dd = home_dd;
htot1_A = home_A;
htot1_Ad = home_Ad;
htot1_Add = home_Add;
t1tot2 = task1;
t1tot2_d = task1_d;
t1tot2_dd = task1_dd;
t1tot2_A = task1_A;
t1tot2_Ad = task1_Ad;
t1tot2_Add = task1_Add;
t2tot3 = task2;
t2tot3_d = task2_d;
t2tot3_dd = task2_dd;

o2E = open;
o2E_d = open_d;
o2E_dd = open_dd;
o2E_A = open_A;
o2E_Ad = open_Ad;
o2E_Add = open_Add;
%%%%%
E2B = E_open;
E2B_d = E_open_d;
E2B_dd = E_open_dd;
E2B_A = E_open_A;
E2B_Ad = E_open_Ad;
E2B_Add = E_open_Add;
%%%%%
B2h = B_open;
B2h_d = B_open_d;
B2h_dd = B_open_dd;
B2h_A = B_open_A;
B2h_Ad = B_open_Ad;
B2h_Add = B_open_Add;

%Settings for robot (Schunk' s robotic arm)

new(1)=Link([0 0 0 -pi/2]);
new(2)=Link([0 0 0.350 pi]);
new(2).offset=-pi/2;
new(3)=Link([0 0 0 -pi/2]);
new(3).offset=-pi/2;
new(4)=Link([0 0.305 0 pi/2]);
new(5)=Link([0 0 0 -pi/2]);
new(6)=Link([0 0.320 0 0]);

final = SerialLink(new, 'name', 'final')


%Quintic polynomial trajectories
Qnt = [1, IT, IT.^2, IT.^3, IT.^4, IT.^5;                                                           %representing equation in matrix
     0, 1,  2*IT, 3*IT.^2, 4*IT.^3, 5*IT.^4;                                                        %inital and termial time
     0, 0, 2, 6*IT, 12*IT.^2, 20*IT.^3;
     1, FT, FT.^2, FT.^3, FT.^4, FT.^5 ;                                                          %representing equation in matrix
     0, 1,  2*FT, 3*FT.^2, 4*FT.^3, 5*FT.^4;                                                        %inital and termial time
     0, 0, 2, 6*FT, 12*FT.^2, 20*FT.^3;
     ];

cnst_h2t1(:,1) = Qnt\[home(1);home_d(1);home_dd(1);task1(1);task1_d(1);task1_dd(1)];
cnst_h2t1(:,2) = Qnt\[home(2);home_d(2);home_dd(2);task1(2);task1_d(2);task1_dd(2)];
cnst_h2t1(:,3) = Qnt\[home(3);home_d(3);home_dd(3);task1(3);task1_d(3);task1_dd(3)];
cnst_h2t1_A(:,1) = Qnt\[home_A(1);home_Ad(1);home_Add(1);task1_A(1);task1_Ad(1);task1_Add(1)];
cnst_h2t1_A(:,2) = Qnt\[home_A(2);home_Ad(2);home_Add(2);task1_A(2);task1_Ad(2);task1_Add(2)];
cnst_h2t1_A(:,3) = Qnt\[home_A(3);home_Ad(3);home_Add(3);task1_A(3);task1_Ad(3);task1_Add(3)];

cnst_t12t2(:,1) = Qnt\[task1(1);task1_d(1);task1_dd(1);task2(1);task2_d(1);task2_dd(1)];
cnst_t12t2(:,2) = Qnt\[task1(2);task1_d(2);task1_dd(2);task2(2);task2_d(2);task2_dd(2)];
cnst_t12t2(:,3) = Qnt\[task1(3);task1_d(3);task1_dd(3);task2(3);task2_d(3);task2_dd(3)];
cnst_t12t2_A(:,1) = Qnt\[task1_A(1);task1_Ad(1);task1_Add(1);task2_A(1);task2_Ad(1);task2_Add(1)];
cnst_t12t2_A(:,2) = Qnt\[task1_A(2);task1_Ad(2);task1_Add(2);task2_A(2);task2_Ad(2);task2_Add(2)];
cnst_t12t2_A(:,3) = Qnt\[task1_A(3);task1_Ad(3);task1_Add(3);task2_A(3);task2_Ad(3);task2_Add(3)];

cnst_o2E(:,1) = Qnt\[open(1);open_d(1);open_dd(1);E_open(1);E_open_d(1);E_open_dd(1)];
cnst_o2E(:,2) = Qnt\[open(2);open_d(2);open_dd(2);E_open(2);E_open_d(2);E_open_dd(2)];
cnst_o2E(:,3) = Qnt\[open(3);open_d(3);open_dd(3);E_open(3);E_open_d(3);E_open_dd(3)];
cnst_o2E_A(:,1) = Qnt\[open_A(1);open_Ad(1);open_Add(1);E_open_A(1);E_open_Ad(1);E_open_Add(1)];
cnst_o2E_A(:,2) = Qnt\[open_A(2);open_Ad(2);open_Add(2);E_open_A(2);E_open_Ad(2);E_open_Add(2)];
cnst_o2E_A(:,3) = Qnt\[open_A(3);open_Ad(3);open_Add(3);E_open_A(3);E_open_Ad(3);E_open_Add(3)];
%%%%%
cnst_E2B(:,1) = Qnt\[E_open(1);E_open_d(1);E_open_dd(1);B_open(1);B_open_d(1);B_open_dd(1)];
cnst_E2B(:,2) = Qnt\[E_open(2);E_open_d(2);E_open_dd(2);B_open(2);B_open_d(2);B_open_dd(2)];
cnst_E2B(:,3) = Qnt\[E_open(3);E_open_d(3);E_open_dd(3);B_open(3);B_open_d(3);B_open_dd(3)];
cnst_E2B_A(:,1) = Qnt\[E_open_A(1);E_open_Ad(1);E_open_Add(1);B_open_A(1);B_open_Ad(1);B_open_Add(1)];
cnst_E2B_A(:,2) = Qnt\[E_open_A(2);E_open_Ad(2);E_open_Add(2);B_open_A(2);B_open_Ad(2);B_open_Add(2)];
cnst_E2B_A(:,3) = Qnt\[E_open_A(3);E_open_Ad(3);E_open_Add(3);B_open_A(3);B_open_Ad(3);B_open_Add(3)];

%%%%%
cnst_B2h(:,1) = Qnt\[B_open(1);B_open_d(1);B_open_dd(1);open(1);open_d(1);open_dd(1)];
cnst_B2h(:,2) = Qnt\[B_open(2);B_open_d(2);B_open_dd(2);open(2);open_d(2);open_dd(2)];
cnst_B2h(:,3) = Qnt\[B_open(3);B_open_d(3);B_open_dd(3);open(3);open_d(3);open_dd(3)];
cnst_B2h_A(:,1) = Qnt\[B_open_A(1);B_open_Ad(1);B_open_Add(1);open_A(1);open_Ad(1);open_Add(1)];
cnst_B2h_A(:,2) = Qnt\[B_open_A(2);B_open_Ad(2);B_open_Add(2);open_A(2);open_Ad(2);open_Add(2)];
cnst_B2h_A(:,3) = Qnt\[B_open_A(3);B_open_Ad(3);B_open_Add(3);open_A(3);open_Ad(3);open_Add(3)];


for i = 0 : s_time : FT
    htot1_d = cnst_h2t1(2,:) + 2 * cnst_h2t1(3,:)*time + 3*cnst_h2t1(4,:)*time.^2 + 4*cnst_h2t1(5,:)*time.^3 + 5*cnst_h2t1(6,:)*time.^4 ;
    htot1_Ad = cnst_h2t1_A(2,:) + 2 * cnst_h2t1_A(3,:)*time + 3*cnst_h2t1_A(4,:)*time.^2 + 4*cnst_h2t1_A(5,:)*time.^3 + 5*cnst_h2t1_A(6,:)*time.^4 ;
    htot1 = htot1 + s_time*htot1_d';
    htot1_A = htot1_A + s_time*htot1_Ad';

    t1tot2_d = cnst_t12t2(2,:) + 2 * cnst_t12t2(3,:)*time + 3*cnst_t12t2(4,:)*time.^2 + 4*cnst_t12t2(5,:)*time.^3 + 5*cnst_t12t2(6,:)*time.^4 ;
    t1tot2_Ad = cnst_t12t2_A(2,:) + 2 * cnst_t12t2_A(3,:)*time + 3*cnst_t12t2_A(4,:)*time.^2 + 4*cnst_t12t2_A(5,:)*time.^3 + 5*cnst_t12t2_A(6,:)*time.^4 ;
    t1tot2 = t1tot2 + s_time*t1tot2_d';
    t1tot2_A = t1tot2_A + s_time*t1tot2_Ad';
    
    o2E_d = cnst_o2E(2,:) + 2 * cnst_o2E(3,:)*time + 3*cnst_o2E(4,:)*time.^2 + 4*cnst_o2E(5,:)*time.^3 + 5*cnst_o2E(6,:)*time.^4 ;
    o2E_Ad = cnst_o2E_A(2,:) + 2 * cnst_o2E_A(3,:)*time + 3*cnst_o2E_A(4,:)*time.^2 + 4*cnst_o2E_A(5,:)*time.^3 + 5*cnst_o2E_A(6,:)*time.^4 ;
    o2E = o2E + s_time*o2E_d';
    o2E_A = o2E_A + s_time*o2E_Ad';
    %%%%%
    E2B_d = cnst_E2B(2,:) + 2 * cnst_E2B(3,:)*time + 3*cnst_E2B(4,:)*time.^2 + 4*cnst_E2B(5,:)*time.^3 + 5*cnst_E2B(6,:)*time.^4 ;
    E2B = E2B + s_time*E2B_d';
    E2B_Ad = cnst_E2B_A(2,:) + 2 * cnst_E2B_A(3,:)*time + 3*cnst_E2B_A(4,:)*time.^2 + 4*cnst_E2B_A(5,:)*time.^3 + 5*cnst_E2B_A(6,:)*time.^4 ;
    E2B_A = E2B_A + s_time*E2B_Ad';
    %%%%%
    B2h_d = cnst_B2h(2,:) + 2 * cnst_B2h(3,:)*time + 3*cnst_B2h(4,:)*time.^2 + 4*cnst_B2h(5,:)*time.^3 + 5*cnst_B2h(6,:)*time.^4 ;
    B2h_Ad = cnst_B2h_A(2,:) + 2 * cnst_B2h_A(3,:)*time + 3*cnst_B2h_A(4,:)*time.^2 + 4*cnst_B2h_A(5,:)*time.^3 + 5*cnst_B2h_A(6,:)*time.^4 ;
    B2h = B2h + s_time*B2h_d';
    B2h_A = B2h_A + s_time*B2h_Ad';
    
    htot1x_save(n) = htot1(1);
    htot1y_save(n) = htot1(2);
    htot1z_save(n) = htot1(3);
    htot1Ax_save(n) = htot1_A(1); %gamma
    htot1Ay_save(n) = htot1_A(2); %beta
    htot1Az_save(n) = htot1_A(3); %alpha
     
    t1tot2x_save(n) = t1tot2(1);
    t1tot2y_save(n) = t1tot2(2);
    t1tot2z_save(n) = t1tot2(3);
    t1tot2Ax_save(n) = t1tot2_A(1); %gamma
    t1tot2Ay_save(n) = t1tot2_A(2); %beta
    t1tot2Az_save(n) = t1tot2_A(3); %alpha
    
    o2Ex_save(n) = o2E(1);
    o2Ey_save(n) = o2E(2);
    o2Ez_save(n) = o2E(3);
    o2EAx_save(n) = o2E_A(1); %gamma
    o2EAy_save(n) = o2E_A(2); %beta
    o2EAz_save(n) = o2E_A(3); %alpha
    %%%%%
    E2Bx_save(n) = E2B(1);
    E2By_save(n) = E2B(2);
    E2Bz_save(n) = E2B(3);
    E2BAx_save(n) = E2B_A(1); %gamma
    E2BAy_save(n) = E2B_A(2); %beta
    E2BAz_save(n) = E2B_A(3); %alpha
    %%%%%
    B2hx_save(n) = B2h(1);
    B2hy_save(n) = B2h(2);
    B2hz_save(n) = B2h(3);
    B2hAx_save(n) = B2h_A(1); %gamma
    B2hAy_save(n) = B2h_A(2); %beta
    B2hAz_save(n) = B2h_A(3); %alpha
    
    
    htot1_dx_save(n) = htot1_d(1);
    htot1_dy_save(n) = htot1_d(2);
    htot1_dz_save(n) = htot1_d(3);
   
    htot1Adx_save(n) = htot1_Ad(1);
    htot1Ady_save(n) = htot1_Ad(2);
    htot1Adz_save(n) = htot1_Ad(3);
    
    t1tot2_dx_save(n) = t1tot2_d(1);
    t1tot2_dy_save(n) = t1tot2_d(2);
    t1tot2_dz_save(n) = t1tot2_d(3);
   
    t1tot2Adx_save(n) = t1tot2_Ad(1);
    t1tot2Ady_save(n) = t1tot2_Ad(2);
    t1tot2Adz_save(n) = t1tot2_Ad(3);
    
    o2E_dx_save(n) = o2E_d(1);
    o2E_dy_save(n) = -0.68/FT;%% o2E_d(2);
    o2E_dz_save(n) = o2E_d(3);
   
    o2EAdx_save(n) = o2E_Ad(1);
    o2EAdy_save(n) = o2E_Ad(2);
    o2EAdz_save(n) = o2E_Ad(3);
   %%%%%
    E2B_dx_save(n) = 0.3/FT%%E2B_d(1);
    E2B_dy_save(n) = E2B_d(2);
    E2B_dz_save(n) = E2B_d(3);
    E2BAdx_save(n) = E2B_Ad(1);
    E2BAdy_save(n) = E2B_Ad(2);
    E2BAdz_save(n) = E2B_Ad(3);
    %%%%%   
    B2h_dx_save(n) = B2h_d(1);
    B2h_dy_save(n) = B2h_d(2);
    B2h_dz_save(n) = B2h_d(3);
  
    B2hAdx_save(n) = B2h_Ad(1);
    B2hAdy_save(n) = B2h_Ad(2);
    B2hAdz_save(n) = B2h_Ad(3);
   
    
    n = n+1;
    time = time + s_time;
end

% result of trajectory in graph
T = 0:s_time:FT;
figure(1)
subplot(2,1,1)
plot(T, htot1x_save, T, htot1y_save,'r--',T, htot1z_save,'g--')
legend('X','Y','Z')
xlabel('Time (sec)')
ylabel('Work space coordination (m)')
axis([0 2 0 1])
subplot(2,1,2)
plot(T, htot1_dx_save, T, htot1_dy_save,'r--',T, htot1_dz_save,'g--')
xlabel('Time (sec)')
ylabel('Work space vel.(m/s)')
axis([0 2 -2 2])
figure(2)
subplot(2,1,1)
plot(T, htot1Ax_save*180/pi, T, htot1Ay_save*180/pi,'r--',T, htot1Az_save*180/pi,'g--')
xlabel('Time (sec)')
ylabel('EE angle (degree)')
legend('X','Y','Z')
subplot(2,1,2)
plot(T, htot1Adx_save*180/pi, T, htot1Ady_save*180/pi,'r--',T, htot1Adz_save*180/pi,'g--')
xlabel('Time (sec)')
ylabel('EE angle vel.(degree/s)')


n = 1;

for i = 0 : s_time : FT

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));


%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = ( Qs' + J\[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);

X_save(n) = T0_6(1,4);
Y_save(n) = T0_6(2,4);
Z_save(n) = T0_6(3,4);


beta = atan2(-T0_6(3,1),(T0_6(1,1)^2+T0_6(2,1)^2)^(1/2));
if cos(beta) == 0
    alpha = 0;
    if beta>0
        gamma = -atan2(T0_6(1,2),T0_6(2,2));
    else
        gamma = atan2(T0_6(1,2),T0_6(2,2));
    
    end
else
    alpha = atan2(T0_6(2,1)/cos(beta), T0_6(1,1)/cos(beta));
    gamma = atan2(T0_6(3,2)/cos(beta), T0_6(3,3)/cos(beta));
end


Xa_save(n) = alpha;
Ya_save(n) = beta;
Za_save(n) = gamma;

n = n+1
end

T = 0:s_time:FT;
figure(3)
h(1) = surface(xcy-0.55, -zcy*0.25-0.1, ycy)
%final.plot(Qs_save*pi/180/1000)

j=1
for i = 0 : s_time : FT

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = ( Qs' + J\[t1tot2_dx_save(j) t1tot2_dy_save(j) t1tot2_dz_save(j) t1tot2Adx_save(j) t1tot2Ady_save(j) t1tot2Adz_save(j)]'*s_time )';


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);

X_save(n) = T0_6(1,4);
Y_save(n) = T0_6(2,4);
Z_save(n) = T0_6(3,4);

beta = atan2(-T0_6(3,1),(T0_6(1,1)^2+T0_6(2,1)^2)^(1/2));
if cos(beta) == 0
    alpha = 0;
    if beta>0
        gamma = -atan2(T0_6(1,2),T0_6(2,2));
    else
        gamma = atan2(T0_6(1,2),T0_6(2,2));
    
    end
else
    alpha = atan2(T0_6(2,1)/cos(beta), T0_6(1,1)/cos(beta));
    gamma = atan2(T0_6(3,2)/cos(beta), T0_6(3,3)/cos(beta));
end


Xa_save(n) = alpha;
Ya_save(n) = beta;
Za_save(n) = gamma;
j=j+1
n = n+1
end

% for animation
T = 0:s_time:FT;
figure(3)
h(1) = surface(xcy-0.55, -zcy*0.25-0.1, ycy)
%final.plot(Qs_save/180*pi/1000)

j=1
% task2 to task3(Opening the handle)
for i = 0 : 23

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

%Generate Jacobian
z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

% Calculrate joint values 
%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = (Qs + (J\[p2top3(1,1,j+1)-p2top3(1,1,j);p2top3(2,1,j+1)-p2top3(2,1,j);p2top3(3,1,j+1)-p2top3(3,1,j); a2toa3(1,1,j+1)-a2toa3(1,1,j);a2toa3(2,1,j+1)-a2toa3(2,1,j);a2toa3(3,1,j+1)-a2toa3(3,1,j)])');


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);
j = j+1
n = n+1
end

j=1
% task3 to task4(releasing the handle)
for i = 0 : 23

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

%Generate Jacobian
z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

% Calculrate joint values 
%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = (Qs + (J\[p3top4(1,1,j+1)-p3top4(1,1,j);p3top4(2,1,j+1)-p3top4(2,1,j);p3top4(3,1,j+1)-p3top4(3,1,j); a3toa4(1,1,j+1)-a3toa4(1,1,j);a3toa4(2,1,j+1)-a3toa4(2,1,j);a3toa4(3,1,j+1)-a3toa4(3,1,j)])');


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);
j = j+1
n = n+1
end

j= 1;

for i = 0 : s_time : FT

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

%Qs = ( Qs' + J_d*[o2E_dx_save(n) o2E_dy_save(n) o2E_dz_save(n) o2EAdx_save(n) o2EAdy_save(n) o2EAdz_save(n)]'*s_time )';
Qs = ( Qs' + J\[o2E_dx_save(j) o2E_dy_save(j) o2E_dz_save(j) o2EAdx_save(j) o2EAdy_save(j) o2EAdz_save(j)]'*s_time )';


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);

X_save(n) = T0_6(1,4);
Y_save(n) = T0_6(2,4);
Z_save(n) = T0_6(3,4);


beta = atan2(-T0_6(3,1),(T0_6(1,1)^2+T0_6(2,1)^2)^(1/2));
if cos(beta) == 0
    alpha = 0;
    if beta>0
        gamma = -atan2(T0_6(1,2),T0_6(2,2));
    else
        gamma = atan2(T0_6(1,2),T0_6(2,2));
    
    end
else
    alpha = atan2(T0_6(2,1)/cos(beta), T0_6(1,1)/cos(beta));
    gamma = atan2(T0_6(3,2)/cos(beta), T0_6(3,3)/cos(beta));
end


Xa_save(n) = alpha;
Ya_save(n) = beta;
Za_save(n) = gamma;

j = j+1
n = n+1
end
%%%%%
j=1
for i = 0 : s_time : FT

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = ( Qs' + J\[E2B_dx_save(j) E2B_dy_save(j) E2B_dz_save(j) E2BAdx_save(j) E2BAdy_save(j) E2BAdz_save(j)]'*s_time )';


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);

X_save(n) = T0_6(1,4);
Y_save(n) = T0_6(2,4);
Z_save(n) = T0_6(3,4);

beta = atan2(-T0_6(3,1),(T0_6(1,1)^2+T0_6(2,1)^2)^(1/2));
if cos(beta) == 0
    alpha = 0;
    if beta>0
        gamma = -atan2(T0_6(1,2),T0_6(2,2));
    else
        gamma = atan2(T0_6(1,2),T0_6(2,2));
    
    end
else
    alpha = atan2(T0_6(2,1)/cos(beta), T0_6(1,1)/cos(beta));
    gamma = atan2(T0_6(3,2)/cos(beta), T0_6(3,3)/cos(beta));
end


Xa_save(n) = alpha;
Ya_save(n) = beta;
Za_save(n) = gamma;
j=j+1
n = n+1
end

%%%%%
j=1
for i = 0 : s_time : FT

T0_1=[cos(q1)      0        -sin(q1)   0
      sin(q1)      0        cos(q1)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T1_2=[cos(q2-pi/2)      sin(q2-pi/2)     0       0.35*cos(q2-pi/2)   
      sin(q2-pi/2)     -cos(q2-pi/2)     0       0.35*sin(q2-pi/2)
           0       0          -1       0        
           0       0           0       1      ];


T2_3=[cos(q3-pi/2)      0        -sin(q3-pi/2)   0
      sin(q3-pi/2)      0        cos(q3-pi/2)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T3_4=[cos(q4)    0        sin(q4)      0  
      sin(q4)    0       -cos(q4)      0
        0        1          0          0.305
        0        0          0          1     ];
      
T4_5=[cos(q5)      0        -sin(q5)   0
      sin(q5)      0        cos(q5)    0 
          0        -1          0       0         
          0        0           0       1      ]; 
      
T5_6=[cos(q6)     -sin(q6)     0       0   
      sin(q6)     cos(q6)      0       0
           0       0           1       0.320        
           0       0           0       1      ];

%*****Forward kinematics****
T0_2 = T0_1 * T1_2;
T0_3 = T0_1 * T1_2 * T2_3;
T0_4 = T0_1 * T1_2 * T2_3 * T3_4;
T0_5 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5;
T0_6 = T0_1 * T1_2 * T2_3 * T3_4 * T4_5 * T5_6;

z0=[0 0 1]';
z1=T0_1(1:3,3);
z2=T0_2(1:3,3);
z3=T0_3(1:3,3);
z4=T0_4(1:3,3);
z5=T0_5(1:3,3);
z6=T0_6(1:3,3);

J(1:3,1)=cross(z0, (T0_6(1:3,4)-[0   0   0]'));
J(4:6,1)=z0;

J(1:3,2)=cross( z1, (T0_6(1:3,4)-T0_1(1:3,4)));
J(4:6,2)=z1;

J(1:3,3)=cross(z2, (T0_6(1:3,4)-T0_2(1:3,4)));
J(4:6,3)=z2;

J(1:3,4)=cross(z3, (T0_6(1:3,4)-T0_3(1:3,4)));
J(4:6,4)=z3;

J(1:3,5)=cross(z4, (T0_6(1:3,4)-T0_4(1:3,4)));
J(4:6,5)=z4;

J(1:3,6)=cross(z5, (T0_6(1:3,4)-T0_5(1:3,4)));
J(4:6,6)=z5;

ramda = 0.1;
J_d=J'/(J*J'+ramda^2*eye(6,6));
J

%Qs = ( Qs' + J_d*[htot1_dx_save(n) htot1_dy_save(n) htot1_dz_save(n) htot1Adx_save(n) htot1Ady_save(n) htot1Adz_save(n)]'*s_time )';
Qs = ( Qs' + J\[B2h_dx_save(j) B2h_dy_save(j) B2h_dz_save(j) B2hAdx_save(j) B2hAdy_save(j) B2hAdz_save(j)]'*s_time )';


Qs_save(n,:) = Qs*180/pi*1000;
q1 = Qs(1);
q2 = Qs(2);
q3 = Qs(3);
q4 = Qs(4);
q5 = Qs(5);
q6 = Qs(6);

X_save(n) = T0_6(1,4);
Y_save(n) = T0_6(2,4);
Z_save(n) = T0_6(3,4);

beta = atan2(-T0_6(3,1),(T0_6(1,1)^2+T0_6(2,1)^2)^(1/2));
if cos(beta) == 0
    alpha = 0;
    if beta>0
        gamma = -atan2(T0_6(1,2),T0_6(2,2));
    else
        gamma = atan2(T0_6(1,2),T0_6(2,2));
    
    end
else
    alpha = atan2(T0_6(2,1)/cos(beta), T0_6(1,1)/cos(beta));
    gamma = atan2(T0_6(3,2)/cos(beta), T0_6(3,3)/cos(beta));
end


Xa_save(n) = alpha;
Ya_save(n) = beta;
Za_save(n) = gamma;
j=j+1
n = n+1
end


% for animation and plotting
T = 0:s_time:FT;
figure(3)
h(1) = surface(xcy-0.55, -zcy*0.25-0.1, ycy)
final.plot(Qs_save/180*pi/1000)
% figure(4)
% subplot(2,2,1)
% axis([0 2 -0.3 0.01])
% plot(T,X_save);
% legend('x')
% xlabel('Time (sec)')
% ylabel('Work space coordination (m)')
% subplot(2,2,2)
% axis([0 2 0 0.02])
% plot(T,Y_save,'r--')
% legend('y')
% xlabel('Time (sec)')
% ylabel('Work space coordination (m)')
% subplot(2,2,3)
% axis([0 2 0.5 0.8])
% plot(T,Z_save,'g--')
% legend('z')
% xlabel('Time (sec)')
% ylabel('Work space coordination (m)')
% figure(5)
% subplot(2,2,1)
% plot(T,Xa_save*180/pi);
% legend('x')
% xlabel('Time (sec)')
% ylabel('Work space coordination (dgr)')
% subplot(2,2,2)
% plot(T,Ya_save*180/pi);
% legend('y')
% xlabel('Time (sec)')
% ylabel('Work space coordination (dgr)')
% subplot(2,2,3)
% plot(T,Za_save*180/pi);
% legend('z')
% xlabel('Time (sec)')
% ylabel('Work space coordination (dgr)')

















