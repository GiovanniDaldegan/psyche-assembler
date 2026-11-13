; comentário teste
section TEXT
INPUT OLD_DATA        		;teste espaço e tab
LOAD OLD_DATA
L1: DIV DOIS
;asds
; asd asd a
;pca   ; asdas
;a

STORE  NEW_DATA
MULT DOIS
STORE TMP_DATA
LOAD OLD_DATA
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
SECTION DATA
DOIS: CONST 2
OLD_DATA: SPACE
NEW_DATA: SPACE
TMP_DATA: SPACE ; penúltimo comentário
; último comentário
;último comentário mesmo
