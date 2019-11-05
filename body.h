const char* body = R"rawText(

<div class="container">
<div class="panel panel-default">
  <div class="panel-heading" align="center">ROBOT CONTROL</div>
  <div class="panel-body" align="center">
      <div class="row">
          <div class="col-xs-4"></div>
          <div class="col-xs-4"><a type="button" href="/front" class="btn btn-primary">&nbsp;&nbsp;&nbsp;&#x2191;&nbsp;&nbsp;&nbsp;</a></div>
          <div class="col-xs-4"></div>
      </div>
      <div class="row">
          <div class="col-xs-4"><a type="button" href="/left" class="btn btn-default">&nbsp;&nbsp;&#x2190;&nbsp;&nbsp;</a></div>
          <div class="col-xs-4"></div>
          <div class="col-xs-4"><a type="button" href="/right" class="btn btn-default">&nbsp;&nbsp;&#x2192;&nbsp;&nbsp;</a></div>
      </div>
      <div class="row">
          <div class="col-xs-4"></div>
          <div class="col-xs-4"><a type="button"  href="/back" class="btn btn-default">&nbsp;&nbsp;&nbsp;&#x2193;&nbsp;&nbsp;&nbsp;</a></div>
          <div class="col-xs-4"></div>
      </div>
  </div>
</div>
</div>

<div class="container">
  <div class="panel panel-default">
      <div class="panel-heading" align="center">LED CONTROL</div>
      <div class="panel-body" align="center">
        <div class="row">
            <div class="col-xs-6"><a href="/L" class="btn btn-success">LED ON</a></div>
            <div class="col-xs-6"><a href="/H" class="btn btn-danger">LED OFF</a></div>
        </div>
      </div>
  </div>
</div> 

)rawText";
