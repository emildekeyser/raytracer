with rootbox as (
    select 1 as n, max(x) as x1, max(y) y1, max(z) as z1,
        min(x) as x2, min(y) as y2, min(z) as z2
    from vertex
    union all
    select n + 1, x1/2, y1/2, z1/2,
        x2/2, y2/2, z2/2
    from rootbox
    where n <= 10
)
select n, x1, y1, z1, x2, y2, z2
from rootbox;
