type checkboxTransitionT =
  | CheckedToUnchecked
  | UncheckedToChecked
  | Nothing;
type checkboxT = {
  checkedImage: Reprocessing.imageT,
  uncheckedImage: Reprocessing.imageT,
  animationState: checkboxTransitionT,
  checked: bool,
  time: float,
  animationTime: float,
  prevPressDown: bool,
};

let opacity = (alpha, env) =>
  Reprocessing.(
    Draw.tint(
      Utils.color(
        ~r=255,
        ~g=255,
        ~b=255,
        ~a=
          int_of_float(
            Utils.remapf(
              ~value=alpha,
              ~low1=0.,
              ~high1=1.,
              ~low2=0.,
              ~high2=255.,
            ),
          ),
      ),
      env,
    )
  );

let drawCheckbox = (~checkbox, ~text="", ~pos as (x, y) as pos, env) => {
  open Reprocessing;
  let size = 28;

  let (mx, my) = Env.mouse(env);
  let pressDown = Env.mousePressed(env);

  let textWidth = Draw.textWidth(~body=text, env);
  let withinBounds =
    mx > x && mx < x + size + textWidth && my > y && my < y + size;
  let clicked = checkbox.prevPressDown && ! pressDown && withinBounds;

  if (text != "") {
    Draw.text(~body=text, ~pos=(x + size + 12, y), env);
  };

  switch (clicked, checkbox.animationState) {
  | (true, CheckedToUnchecked) => {
      ...checkbox,
      animationState: UncheckedToChecked,
      prevPressDown: pressDown,
      checked: true,
      /* @Incomplete Jump to end of animation for simplicity's sake. Ideally we'd use additive animation
         which would give a bit of momentum to the animation of the ticking. It's especially useful
         when syncing the amount ticked to something else. */
      time: 0.,
    }
  | (false, CheckedToUnchecked) =>
    let remapedTime =
      Utils.remapf(
        ~value=checkbox.time,
        ~low1=checkbox.animationTime,
        ~high1=0.,
        ~low2=0.,
        ~high2=1.,
      );

    Draw.image(checkbox.uncheckedImage, ~pos, ~width=size, ~height=size, env);

    Draw.pushMatrix(env);
    opacity(
      Utils.remapf(
        ~value=remapedTime,
        ~low1=0.,
        ~high1=1.,
        ~low2=1.,
        ~high2=0.,
      ),
      env,
    );
    let scale =
      Utils.remapf(
        ~value=remapedTime,
        ~low1=0.,
        ~high1=1.,
        ~low2=1.,
        ~high2=0.8,
      );
    let scaledSize = float_of_int(size) /. 2. *. scale;
    Draw.translate(
      ~x=float_of_int(x) +. scaledSize,
      ~y=float_of_int(y) +. scaledSize,
      env,
    );
    Draw.scale(~x=scale, ~y=scale, env);
    Draw.translate(~x=-. scaledSize, ~y=-. scaledSize, env);
    Draw.image(
      checkbox.checkedImage,
      ~pos=(0, 0),
      ~width=size,
      ~height=size,
      env,
    );
    Draw.popMatrix(env);
    opacity(1., env);

    if (checkbox.time <= 0.) {
      {
        ...checkbox,
        animationState: Nothing,
        time: 0.,
        prevPressDown: pressDown,
      };
    } else {
      {
        ...checkbox,
        time: max(checkbox.time -. Env.deltaTime(env), 0.),
        prevPressDown: pressDown,
      };
    };
  | (true, UncheckedToChecked) => {
      ...checkbox,
      animationState: CheckedToUnchecked,
      prevPressDown: pressDown,
      checked: false,
      /* @Incomplete Jump to end of animation for simplicity's sake. Ideally we'd use additive animation
         which would give a bit of momentum to the animation of the ticking. It's especially useful
         when syncing the amount ticked to something else. */
      time: checkbox.animationTime,
    }
  | (false, UncheckedToChecked) =>
    let remapedTime =
      Utils.remapf(
        ~value=checkbox.time,
        ~low1=0.,
        ~high1=checkbox.animationTime,
        ~low2=0.,
        ~high2=1.,
      );

    Draw.image(checkbox.uncheckedImage, ~pos, ~width=size, ~height=size, env);
    opacity(
      Utils.remapf(
        ~value=remapedTime,
        ~low1=0.,
        ~high1=1.,
        ~low2=0.,
        ~high2=1.,
      ),
      env,
    );
    Draw.pushMatrix(env);
    let scale =
      Utils.remapf(
        ~value=remapedTime,
        ~low1=0.,
        ~high1=1.,
        ~low2=0.8,
        ~high2=1.,
      );
    let scaledSize = float_of_int(size) /. 2. *. scale;
    Draw.translate(
      ~x=float_of_int(x) +. scaledSize,
      ~y=float_of_int(y) +. scaledSize,
      env,
    );
    Draw.scale(~x=scale, ~y=scale, env);
    Draw.translate(~x=-. scaledSize, ~y=-. scaledSize, env);
    Draw.image(
      checkbox.checkedImage,
      ~pos=(0, 0),
      ~width=size,
      ~height=size,
      env,
    );
    Draw.popMatrix(env);
    opacity(1., env);

    if (checkbox.time >= checkbox.animationTime) {
      {
        ...checkbox,
        animationState: Nothing,
        time: checkbox.animationTime,
        prevPressDown: pressDown,
      };
    } else {
      {
        ...checkbox,
        time:
          min(checkbox.time +. Env.deltaTime(env), checkbox.animationTime),
        prevPressDown: pressDown,
      };
    };
  | (true, Nothing) =>
    if (checkbox.checked) {
      Draw.image(checkbox.checkedImage, ~pos, ~width=size, ~height=size, env);

      {
        ...checkbox,
        animationState: CheckedToUnchecked,
        checked: false,
        prevPressDown: pressDown,
      };
    } else {
      Draw.image(
        checkbox.uncheckedImage,
        ~pos,
        ~width=size,
        ~height=size,
        env,
      );
      {
        ...checkbox,
        animationState: UncheckedToChecked,
        checked: true,
        prevPressDown: pressDown,
      };
    }
  | (false, Nothing) =>
    if (checkbox.checked) {
      Draw.image(checkbox.checkedImage, ~pos, ~width=size, ~height=size, env);
    } else {
      Draw.image(
        checkbox.uncheckedImage,
        ~pos,
        ~width=size,
        ~height=size,
        env,
      );
    };
    {...checkbox, prevPressDown: pressDown};
  };
};
let makeCheckbox = (~checked=false, env) => {
  checkedImage:
    Reprocessing.Draw.loadImage(~filename="assets/checked.png", env),
  uncheckedImage:
    Reprocessing.Draw.loadImage(~filename="assets/unchecked.png", env),
  animationState: Nothing,
  checked,
  time: 0.,
  animationTime: 0.2,
  prevPressDown: false,
};

type sliderT = {
  value: float,
  min: float,
  max: float,
  selected: bool,
  deltaXFromCenter: int,
  prevPressDown: bool,
};

let makeSlider = (~defaultValue=50., ~min=0., ~max=100., _env) => {
  value: defaultValue,
  min,
  max,
  selected: false,
  deltaXFromCenter: 0,
  prevPressDown: false,
};

let drawSlider = (~slider, ~pos as (x, y) as pos, env) => {
  open Reprocessing;

  let clickPadding = 6;
  let lineWidth = 4;
  let length = 300;
  let lengthf = float_of_int(length);
  let knobSize = 8;
  let knobSizef = float_of_int(knobSize);

  let endOfLine =
    x + int_of_float((slider.value -. slider.min) /. slider.max *. lengthf);

  let (mx, my) = Env.mouse(env);
  let pressDown = Env.mousePressed(env);
  let (selected, deltaXFromCenter) =
    if (pressDown && ! slider.selected) {
      let deltaX = mx - endOfLine;
      let deltaY = my - y;
      let len = sqrt(float_of_int(deltaX * deltaX + deltaY * deltaY));

      (len < knobSizef, deltaX);
    } else if (pressDown && slider.selected) {
      (true, slider.deltaXFromCenter);
    } else {
      (false, slider.deltaXFromCenter);
    };

  let withinSliderBounds =
    mx > x
    && mx < x
    + length
    && my > y
    - lineWidth
    / 2
    - clickPadding
    && my < y
    + lineWidth
    / 2
    + clickPadding;
  let endOfLine =
    if (selected) {
      mx - deltaXFromCenter;
    } else if (! pressDown && slider.prevPressDown && withinSliderBounds) {
      mx;
    } else {
      endOfLine;
    };

  let endOfLine = max(min(endOfLine, x + length), x);

  Draw.strokeWeight(lineWidth, env);
  Draw.strokeCap(Round, env);
  Draw.stroke(Utils.color(177, 177, 177, 255), env);
  Draw.line(~p1=pos, ~p2=(x + length, y), env);
  Draw.stroke(Utils.color(96, 153, 238, 255), env);
  Draw.line(~p1=pos, ~p2=(endOfLine, y), env);

  Draw.strokeWeight(1, env);
  Draw.stroke(Utils.color(177, 177, 177, 255), env);
  if (selected) {
    Draw.fill(Utils.color(230, 230, 230, 255), env);
  } else {
    Draw.fill(Utils.color(255, 255, 255, 255), env);
  };

  Draw.ellipse(~center=(endOfLine, y), ~radx=knobSize, ~rady=knobSize, env);
  {
    ...slider,
    selected,
    deltaXFromCenter,
    prevPressDown: pressDown,
    value: float_of_int(endOfLine - x) *. slider.max /. lengthf +. slider.min,
  };
};
