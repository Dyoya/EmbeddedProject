-Get-
curl https://port-0-embedded-system-1gksli2alppq2t18.sel4.cloudtype.app

-Post-
curl -d '{"value":35, "time":11}' \                                
-H "Content-Type: application/json" \
-X POST https://port-0-embedded-system-1gksli2alppq2t18.sel4.cloudtype.app/test

이 명령어를 system()이라는 함수에 넣으시면 됩니다
