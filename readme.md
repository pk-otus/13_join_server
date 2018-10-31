# ������� 13
## �������
����������� ����������� ������ ����������� �������� ����������� �
�������������� ������� ��� �����������.
���������� ���������� ������ ������������ ����������� �������� �
����� ��������� ���������� � ������������� ���������. ���� �������
����� �������� A, ������ - B � ����� ���������:
```
{
int id;
std::string name;
}
```
`id` �������� ��������� ������ � �� ����� ������������� � ����� �������.
��� ����������� ������� A:

id | name
---|----------
0 | lean
1 | sweater
2 | frank
3 | violation
4 | quality
5 | precision

� ����������� ������� B:

id | name
---|----------
3 | proposal
4 | example
5 | lake
6 | flour
7 | wonder
8 | selection

���������� ����� ����������� ����������� ������ �� ���� ������ � ����
���������:

id | A | B
---|-----------|---------
3 | violation | proposal
4 | quality | example
5 | precision | lake


���������� ����� ����������� �������������� �������� ������ �� ����
������ � ���� ���������:

id | A | B
---|-----------|---------
0 | lean |
1 | sweater |
2 | frank |
6 | &#xfeff;| flour
7 | &#xfeff;| wonder
8 |	&#xfeff;| selection

������ ������ ��������� � ������� ����������� ���� id.
��� ���������� ��������� � ���������� �������� ��� �����������
���������� ����������� ��������� ��������:
```
INSERT table id name
TRUNCATE table
INTERSECTION
SYMMETRIC_DIFFERENCE
```
## ��������
������� ������������ �� ���� � ��������� ���� �������. ���������
���������� ������� �������� ������ `\n`. � ����� �� ����� ������� ������
��������� ������������������ `OK\n` � ������ ������ � `ERR message\n` �
������ ������ � ������������.
��� ������������ ������ �� ������� ����� ��������� ���������������
��������� �������:
```
> INSERT A 0 lean
< OK
> INSERT A 0 understand
< ERR duplicate 0
> INSERT A 1 sweater
< OK
> INSERT A 2 frank
< OK
...
> INSERT B 6 flour
< OK
> INSERT B 7 wonder
< OK
> INSERT B 8 selection
< OK
```

� �������� ����������� ������������ ������ ���� ������ �������.
������� `TRUNCATE A` � `TRUNCATE B` ������� ��������������� �������.
```
> TRUNCATE A
< OK
```
`INTERSECTION` � `SYMMETRIC_DIFFERENCE` ����� ���������� ������ ������
```
> INTERSECTION
< 3,violation,proposal
< 4,quality,example
< 5,precision,lake
< OK
```
�
```
> SYMMETRIC_DIFFERENCE
< 0,lean,
< 1,sweater,
< 2,frank,
< 6,,flour
< 7,,wonder
< 8,,selection
< OK
```
## ������� �������:
```
# join_server <port>
```
���
� **port** � ����� tcp ����� ��� �������� ����������. ���������� ������
������������� � ������������ �����������.
## ���������� � ����������
����������� ������ ������ ����� ����������� ���� **join_server** �
���������� � ������ **join_server**.
����������� ������ �� ����������� ������� ��������� �� �������, ���
������, ��� ����� ����� ������� ������ �� ���������� ���������� ��
��������������� ����������.
��������� ������ ������ ���� ����������� �� bintray.
## ��������
������� ��������� ����������� �������, ���� ����� ��������� ������ �
������� � ��������� ������� ����� ������������� ��������.
����� �������� ������ ��������� ���������� ����� ������� � �������,
������� � �������� ��� �����������. ���������� ���������� ���
���������� �������� ��������������� ����� �������. ����������
���������� ��� ���������� ���������� �������� ��� �����������.
������������ � ���������� �������� `INSERT` �� ����� ������ ��������
`INTERSECTION` � `SYMMETRIC_DIFFERENCE` ��� �������� ���������� ����
��������.