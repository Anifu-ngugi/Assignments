<%@page contentType="text/html" pageEncoding="GB2312"%>
<html>
<head>
    <title>ͼ��˵�ҳ��</title>
    <%
        String basePath = "";
        basePath = basePath +request.getScheme() + "://" + request.getServerName() +  ":"
                + request.getServerPort() + request.getContextPath() + "/";
        request.setAttribute("basePath",basePath);
    %>
</head>
<body>
<br><br><br>

<button class="btn btn-primary" type="button" style="color: rebeccapurple; background-color: orange;
width: 120px;height: 60px;">
<p><a href="${basePath}book?action=findAll" target="right">��ʾ�鼮</a></p>
</button>
<br><br>

<button class="btn btn-primary" type="button" style="color: rebeccapurple; background-color: orange;
width: 120px;height: 60px;">
<p><a href="${basePath}view/find_push.jsp" target="right">��ѯ�鼮</a></p>
</button>
<br><br>

<button class="btn btn-primary" type="button" style="color: rebeccapurple; background-color: orange;
width: 120px;height: 60px;">
<p><a href="insert_push.jsp" target="right">����鼮</a></p>
</button>
<br><br>

<button class="btn btn-primary" type="button" style="color: rebeccapurple; background-color: orange;
width: 120px;height: 60px;">
<p><a href="../index.jsp" target="_parent">ע����¼</a></p>
</button>
<br><br>

</body>
</html>

